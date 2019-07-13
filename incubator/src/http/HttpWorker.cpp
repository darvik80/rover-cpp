//
// Created by Ivan Kishchenko on 2019-07-13.
//

#include "HttpWorker.h"

#include <boost/beast/version.hpp>
#include <iostream>

// Return a reasonable mime type based on the extension of a file.
boost::beast::string_view mimeType(boost::beast::string_view path) {
    using boost::beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (boost::beast::string_view::npos == pos) {
            return boost::beast::string_view{};
        }

        return path.substr(pos);
    }();
    if (iequals(ext, ".htm")) return "text/html";
    if (iequals(ext, ".html")) return "text/html";
    if (iequals(ext, ".php")) return "text/html";
    if (iequals(ext, ".css")) return "text/css";
    if (iequals(ext, ".txt")) return "text/plain";
    if (iequals(ext, ".js")) return "application/javascript";
    if (iequals(ext, ".json")) return "application/json";
    if (iequals(ext, ".xml")) return "application/xml";
    if (iequals(ext, ".swf")) return "application/x-shockwave-flash";
    if (iequals(ext, ".flv")) return "video/x-flv";
    if (iequals(ext, ".png")) return "image/png";
    if (iequals(ext, ".jpe")) return "image/jpeg";
    if (iequals(ext, ".jpeg")) return "image/jpeg";
    if (iequals(ext, ".jpg")) return "image/jpeg";
    if (iequals(ext, ".gif")) return "image/gif";
    if (iequals(ext, ".bmp")) return "image/bmp";
    if (iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff")) return "image/tiff";
    if (iequals(ext, ".tif")) return "image/tiff";
    if (iequals(ext, ".svg")) return "image/svg+xml";
    if (iequals(ext, ".svgz")) return "image/svg+xml";

    return "application/text";
}

HttpWorker::HttpWorker(tcp::acceptor &acceptor, std::string doc_root)
        : _acceptor(acceptor),
          _docRoot(std::move(doc_root)) {
}

void HttpWorker::start() {
    accept();
    checkDeadline();
}

void HttpWorker::sendFile(boost::beast::string_view target) {
    // Request path must be absolute and not contain "..".
    if (target.empty() || target[0] != '/' || target.find("..") != std::string::npos) {
        sendBadResponse(http::status::not_found, "File not found\r\n");
        return;
    }

    std::string full_path = _docRoot;
    full_path.append(target.data(), target.size());

    http::file_body::value_type file;
    boost::beast::error_code ec;
    file.open(full_path.c_str(), boost::beast::file_mode::read, ec);
    if (ec) {
        sendBadResponse(
                http::status::not_found,
                "File not found\r\n");
        return;
    }

    _fileResponse.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

    _fileResponse->set(http::field::content_type, mimeType(target.to_string()));
    _fileResponse->set(http::field::server, "Beast");
    _fileResponse->body() = std::move(file);
    _fileResponse->result(http::status::ok);
    _fileResponse->keep_alive(false);
    _fileResponse->prepare_payload();

    _fileSerializer.emplace(*_fileResponse);

    http::async_write(
            _socket,
            *_fileSerializer,
            [this](boost::beast::error_code ec, std::size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                _fileSerializer.reset();
                _fileResponse.reset();
                accept();
            }
    );
}

void HttpWorker::checkDeadline() {
    // The deadline may have moved, so check it has really passed.
    if (_requestDeadline.expiry() <= std::chrono::steady_clock::now()) {
        // Close socket to cancel any outstanding operation.
        boost::beast::error_code ec;
        _socket.close();

        // Sleep indefinitely until we're given a new deadline.
        _requestDeadline.expires_at(std::chrono::steady_clock::time_point::max());
    }

    _requestDeadline.async_wait([this](boost::beast::error_code) {
        checkDeadline();
    });
}

void HttpWorker::accept() {
    // Clean up any previous connection.
    boost::beast::error_code ec;
    _socket.close(ec);
    _buffer.consume(_buffer.size());

    _acceptor.async_accept(
            _socket,
            [this](boost::beast::error_code ec) {
                if (ec) {
                    accept();
                } else {
                    // Request must be fully processed within 60 seconds.
                    _requestDeadline.expires_after(std::chrono::seconds(60));

                    readRequest();
                }
            });
}


void HttpWorker::readRequest() {
    // On each read the parser needs to be destroyed and
    // recreated. We store it in a boost::optional to
    // achieve that.
    //
    // Arguments passed to the parser constructor are
    // forwarded to the message object. A single argument
    // is forwarded to the body constructor.
    //
    // We construct the dynamic body with a 1MB limit
    // to prevent vulnerability to buffer attacks.
    //
    _parser.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

    http::async_read(
            _socket,
            _buffer,
            *_parser,
            [this](boost::beast::error_code ec, std::size_t) {
                if (ec) {
                    accept();
                } else {
                    processRequest(_parser->get());
                }
            });
}

void HttpWorker::processRequest(const HttpRequest &req) {
    switch (req.method()) {
        case http::verb::get:
            sendFile(req.target());
            break;
        case http::verb::post: {
            if (req.target() == "/rpc") {
                this->rpc(req);
                break;
            }
        }

        default:
            // We return responses indicating an error if
            // we do not recognize the request method.
            sendBadResponse(
                    http::status::bad_request,
                    "Invalid request-method '" + req.method_string().to_string() + "'\r\n"
            );
            break;
    }
}

void HttpWorker::sendBadResponse(http::status status, std::string const &error) {
    _stringResponse.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

    _stringResponse->result(status);
    _stringResponse->keep_alive(false);
    _stringResponse->set(http::field::server, "Beast");
    _stringResponse->set(http::field::content_type, "text/plain");
    _stringResponse->body() = error;
    _stringResponse->prepare_payload();

    _stringSerializer.emplace(*_stringResponse);

    http::async_write(
            _socket,
            *_stringSerializer,
            [this](boost::beast::error_code ec, std::size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                _stringSerializer.reset();
                _stringResponse.reset();
                accept();
            }
    );
}

void HttpWorker::rpc(const HttpRequest &req) {
    _stringResponse.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

    _stringResponse->keep_alive(false);
    _stringResponse->set(http::field::server, "Beast");

    if (req.method() != http::verb::post) {
        _stringResponse->result(http::status::bad_request);
        _stringResponse->set(http::field::content_type, "text/plain");
        _stringResponse->body() = "json-rpc support only post requests";
    } else if (req.at(http::field::content_type).find("application/json") == std::string::npos) {
        _stringResponse->result(http::status::bad_request);
        _stringResponse->set(http::field::content_type, "text/plain");
        _stringResponse->body() = "json-rpc support only application/json content-type";
    } else {
        _stringResponse->set(http::field::content_type, "application/json; charset=utf-8");
        _stringResponse->result(http::status::ok);
        _stringResponse->body() = R"({"status": "success"})";
    }

    _stringResponse->prepare_payload();
    _stringSerializer.emplace(*_stringResponse);

    http::async_write(
            _socket,
            *_stringSerializer,
            [this](boost::beast::error_code ec, std::size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                _stringSerializer.reset();
                _stringResponse.reset();
                accept();
            }
    );
}


