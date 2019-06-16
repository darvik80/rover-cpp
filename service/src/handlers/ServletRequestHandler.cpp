//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "Poco/Path.h"
#include "Poco/Logger.h"
#include "Poco/File.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/NetException.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/Application.h>
#include <fstream>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>

#include "ServletRequestHandler.h"

class string;

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

namespace handlers {
    void ServletRequestHandler::handleRequest(
            Poco::Net::HTTPServerRequest &request,
            Poco::Net::HTTPServerResponse &response) {
        try {
            _ptrRequest = &request;
            _ptrResponse = &response;

            handleFile(request, response);
        }
        catch (NetException &ex) {
            std::string err = ex.displayText();
            return;
        }
        catch (Exception &ex) {
            std::string err = ex.displayText();
            sendError(err);
            return;
        }
    }

    void ServletRequestHandler::handleFile(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &res) {
        std::string dir = Application::instance().config().getString("application.dir") + "../var/www";

        const auto& uri = req.getURI();

        auto path =	uri.substr(0, uri.find_first_of('?'));
        if (path == "/" || path.empty()) {
            path = "/index.html";
        }

        path = dir + path;
        Path p(path);
        File f(p.toString());
        if(f.exists()) {
            res.setDate(Timestamp());
            res.setChunkedTransferEncoding(true);
            res.setContentLength(f.getSize());

            res.set("Last-Modified", DateTimeFormatter::format(f.getLastModified(), DateTimeFormat::ISO8601_FORMAT));
            const std::string& extension = p.getExtension();
            std::string mediaType;
            if (extension == "png") {
                mediaType = "image/png";
            } else if (extension == "ico") {
                mediaType = "image/vnd.microsoft.icon";
            } else if (extension == "jpg") {
                mediaType = "image/jpg";
            } else if (extension == "html") {
                mediaType = "text/html; chatset=\"utf-8\"";
            } else if (extension == "js") {
                mediaType = "application/javascript; charset=utf-8";
            } else if (extension == "json") {
                mediaType = "application/json; charset=utf-8";
            }
            if ("" != mediaType) {
                res.setContentType(mediaType);
            }
            res.setContentLength(f.getSize());

            std::ifstream istr(path.c_str(), std::ios::binary | std::ios::in);
            if (istr.good())
            {
                StreamCopier::copyStream(istr, res.send());
                res.setStatus(HTTPResponse::HTTP_OK);

            } else {
                throw OpenFileException();
            }
        } else {
            throw NotFoundException();
        }
    }

    void ServletRequestHandler::sendError(const std::string &text, HTTPResponse::HTTPStatus status) {
        _ptrResponse->setStatus(status);
        _ptrResponse->send()
                << "<html><body><h1>An error has occured:<br>"
                << text
                << "</h1></body></html>";
    }
}
