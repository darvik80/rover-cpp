//
// Created by Ivan Kishchenko on 2019-07-13.
//

#include "HttpWorker.h"

#include <boost/beast/version.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <iostream>

using namespace boost;

// Return a reasonable mime type based on the extension of a file.
std::string mimeType(std::string_view path) {
    using beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (std::string_view::npos == pos) {
            return std::string_view{};
        }

        return path.substr(pos);
    }();

    if (algorithm::iequals(ext, ".htm")) return "text/html";
    if (algorithm::iequals(ext, ".html")) return "text/html";
    if (algorithm::iequals(ext, ".php")) return "text/html";
    if (algorithm::iequals(ext, ".css")) return "text/css";
    if (algorithm::iequals(ext, ".txt")) return "text/plain";
    if (algorithm::iequals(ext, ".js")) return "application/javascript";
    if (algorithm::iequals(ext, ".json")) return "application/json";
    if (algorithm::iequals(ext, ".xml")) return "application/xml";
    if (algorithm::iequals(ext, ".swf")) return "application/x-shockwave-flash";
    if (algorithm::iequals(ext, ".flv")) return "video/x-flv";
    if (algorithm::iequals(ext, ".png")) return "image/png";
    if (algorithm::iequals(ext, ".jpe")) return "image/jpeg";
    if (algorithm::iequals(ext, ".jpeg")) return "image/jpeg";
    if (algorithm::iequals(ext, ".jpg")) return "image/jpeg";
    if (algorithm::iequals(ext, ".gif")) return "image/gif";
    if (algorithm::iequals(ext, ".bmp")) return "image/bmp";
    if (algorithm::iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
    if (algorithm::iequals(ext, ".tiff")) return "image/tiff";
    if (algorithm::iequals(ext, ".tif")) return "image/tiff";
    if (algorithm::iequals(ext, ".svg")) return "image/svg+xml";
    if (algorithm::iequals(ext, ".svgz")) return "image/svg+xml";

    return "application/text";
}

HttpWorker::HttpWorker(asio::io_service &service, std::string_view address, uint16_t port, HttpRequestHandlerManager::Ptr handler)
        : _acceptor(service, {asio::ip::make_address(address), port}), _socket(service), _handler(handler), _requestDeadline(service, (std::chrono::steady_clock::time_point::max) ()) {
}

void HttpWorker::start() {
    accept();
    checkDeadline();
}

void HttpWorker::checkDeadline() {
    // The deadline may have moved, so check it has really passed.
    if (_requestDeadline.expiry() <= std::chrono::steady_clock::now()) {
        _socket.close();

        // Sleep indefinitely until we're given a new deadline.
        _requestDeadline.expires_at(std::chrono::steady_clock::time_point::max());
    }

    _requestDeadline.async_wait([this](beast::error_code) {
        checkDeadline();
    });
}

void HttpWorker::accept() {
    // Clean up any previous connection.
    beast::error_code ec;
    _socket.close(ec);
    _buffer.consume(_buffer.size());

    _acceptor.async_accept(
            _socket,
            [this](beast::error_code ec) {
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
    _parser.emplace();

    http::async_read(
            _socket,
            _buffer,
            *_parser,
            [this](beast::error_code ec, size_t) {
                if (ec) {
                    accept();
                } else {
                    processRequest(_parser->get());
                }
            });
}

void HttpWorker::processRequest(const HttpRequest &req) {
    try {
        _response.emplace();
        _handler->process(req, _response.get());
        _serializer.emplace(*_response);
        http::async_write(
                _socket,
                *_serializer,
                [this](beast::error_code ec, size_t) {
                    _socket.shutdown(tcp::socket::shutdown_send, ec);
                    _serializer.reset();
                    _response.reset();
                    accept();
                }
        );
    } catch (std::invalid_argument& ex) {
        sendBadResponse(http::status::not_found, ex.what());
    } catch (std::exception& ex) {
        sendBadResponse(http::status::bad_request, ex.what());
    }
}

void HttpWorker::sendBadResponse(http::status status, std::string_view error) {
    _response.emplace();

    _response->result(status);
    _response->keep_alive(false);
    _response->set(http::field::server, "Beast");
    _response->set(http::field::content_type, "text/plain");
    _response->body() = error;
    _response->prepare_payload();

    _serializer.emplace(*_response);

    http::async_write(
            _socket,
            *_serializer,
            [this](beast::error_code ec, size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                _serializer.reset();
                _response.reset();
                accept();
            }
    );
}

void HttpWorker::rpc(const HttpRequest &req) {
    _response.emplace();

    _response->keep_alive(false);
    _response->set(http::field::server, "Beast");

    if (req.method() != http::verb::post) {
        _response->result(http::status::bad_request);
        _response->set(http::field::content_type, "text/plain");
        _response->body() = "json-rpc support only post requests";
    } else if (req.at(http::field::content_type).find("application/json") == std::string::npos) {
        _response->result(http::status::bad_request);
        _response->set(http::field::content_type, "text/plain");
        _response->body() = "json-rpc support only application/json content-type";
    } else {
        // TODO:
//        std::shared_ptr<JsonRpcRequest> request = std::make_shared<JsonRpcRequest>();
//        std::shared_ptr<JsonRpcResponse> response = std::make_shared<JsonRpcResponse>();
//        try {
//            JsonDecoder(request).decode(req.body());
//
//            _rpcHandler->handle(*request, *response);
//
//        } catch (std::exception &ex) {
//            JsonRcpError error;
//            error.code = InternalError;
//            error.message = ex.what();
//            response->error = error;
//        }
//
//        _response->body() = JsonEncoder(response).encode();
    }

    _response->prepare_payload();
    _serializer.emplace(*_response);

    http::async_write(
            _socket,
            *_serializer,
            [this](beast::error_code ec, size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                _serializer.reset();
                _response.reset();
                accept();
            }
    );
}

void HttpWorker::shutdown() {
    _acceptor.close();
}

