#include "Server.h"

#include "handlers/Factory.h"

#include <iostream>
#include <string>
#include <sstream>
#include <strstream>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocketImpl.h>
#include <Poco/Util/ServerApplication.h>
#include <rpc/RpcRegistry.h>
#include <rpc/HealthRpcSupplier.h>

#include "handlers/JsonRpcHandler.h"

#include <Poco/ClassLoader.h>
#include "plugin/Module.h"

namespace {

    class ServerSocketImpl : public Poco::Net::ServerSocketImpl {
    public:
        using Poco::Net::SocketImpl::init;
    };

    class Socket : public Poco::Net::Socket {
    public:
        Socket(const std::string &address, short port)
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

int Server::main(const std::vector<std::string> &args) {
    this->logger().information(this->config().getString("application.dir"));
    loadConfiguration(this->config().getString("application.dir")+"../etc/service.properties");

    Poco::Net::HTTPServerParams::Ptr parameters = new Poco::Net::HTTPServerParams();
    parameters->setTimeout(10000);
    parameters->setMaxQueued(100);
    parameters->setMaxThreads(4);

    short port = config().getInt("http.port", 9000);

    const Poco::Net::ServerSocket socket(Socket("localhost", port));

    Poco::Net::HTTPServer server(new handlers::Factory(), socket, parameters);

    server.start();
    waitForTerminationRequest();
    server.stopAll();

    return 0;
}

void Server::initialize(Poco::Util::Application &self) {
    Application::initialize(self);

    // Load Plugins
    Poco::ClassLoader<Module> cl;

    std::string path = "../lib";

    try
    {
        cl.loadLibrary(path);
        if (cl.isLibraryLoaded(path)) {
            for(auto iter = cl.begin(); iter != cl.end(); ++iter) {
                Module* pModule = cl.classFor(iter->first).create();
                std::cout << pModule->name();
                delete pModule;
            }
            cl.unloadLibrary(path);
        }
    }
    catch(Poco::LibraryLoadException& exc)
    {
        std::cout << (Poco::format(" %s \"%s\"", std::string(exc.name()), path));
    }


    RpcRegistry::instance().addMethod(new HealthRpcSupplier());
}
