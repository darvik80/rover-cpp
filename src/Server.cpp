#include "Server.h"

#include "handlers/Factory.h"

#include <iostream>
#include <string>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocketImpl.h>

#include <Poco/Util/ServerApplication.h>
#include "handlers/HandlerJsonRpc.h"

#include <sstream>
#include <strstream>

#include "codec/json/Samples.h"

namespace
{

    class ServerSocketImpl: public Poco::Net::ServerSocketImpl
    {
    public:
        using Poco::Net::SocketImpl::init;
    };

    class Socket: public Poco::Net::Socket
    {
    public:
        Socket(const std::string & address, short port)
                : Poco::Net::Socket(new ServerSocketImpl()) {
            const Poco::Net::SocketAddress socket_address(address, port);
            auto *socket = dynamic_cast<ServerSocketImpl *>(impl());
            socket->init(socket_address.af());
            socket->setReuseAddress(true);
            socket->setReusePort(false);
            socket->bind(socket_address, false);
            socket->listen();
        }
    };

} // anonymous namespace

int Server::main(const std::vector<std::string>& args) {
    /*
    TestMessage test;
    test.id = 1231244;
    test.message.message="Hello";

    std::string res = JsonEncoder(test.marshaller()).encode();

    Message message;
    message.id = 12;
    message.message = "Hello World";

    handlers::JsonRpcRequest request;
    request.id = "124124324235";
    request.method = "ping";
    request.jsonrpc = "2.0";

    request.params = Message::JsonMarshaller<Message>(message).marshal();

    res = JsonEncoder(request.marshaller()).encode();

    handlers::JsonRpcRequest result;
    auto is = std::istringstream(res);
    JsonDecoder(result.unMarshaller()).decode(is);

    Controller controller;
    controller.id = 100;
    controller.config.id = 200;
    controller.config.message = "test";
    res = JsonEncoder(controller.marshaller()).encode();
     */

    loadConfiguration();

    Poco::Net::HTTPServerParams::Ptr parameters = new Poco::Net::HTTPServerParams();
    parameters->setTimeout(10000);
    parameters->setMaxQueued(100);
    parameters->setMaxThreads(4);

    short port = config().getInt("rover.port", 9000);

    const Poco::Net::ServerSocket socket(Socket("localhost", port));

    Poco::Net::HTTPServer server(new handlers::Factory(), socket, parameters);

    server.start();
    waitForTerminationRequest();
    server.stopAll();

    return 0;
}
