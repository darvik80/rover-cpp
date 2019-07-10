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
#include <Poco/Manifest.h>
#include "plugin/Module.h"

#include <Poco/DirectoryIterator.h>
#include <Poco/String.h>

#include <boost/algorithm/string/predicate.hpp>

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

    std::string path = this->config().getString("application.dir") + "../lib";

    Poco::DirectoryIterator end;
    for (Poco::DirectoryIterator it(path); it != end; ++it) {
        if (it->isFile() && boost::algorithm::ends_with(it->path(), Poco::SharedLibrary::suffix())) {
            std::cout << it->path() << std::endl;

            try {
                cl.loadLibrary(it->path());
                if (cl.isLibraryLoaded(it->path())) {
                    for(auto iter = cl.begin(); iter != cl.end(); ++iter) {
                        std::cout << "Lib: " << iter->first << std::endl;
                        Poco::Manifest<Module>::Iterator itMan(iter->second->begin());
                        Poco::Manifest<Module>::Iterator endMan(iter->second->end());
                        for (; itMan != endMan; ++itMan) {
                            std::cout << itMan->name() << std::endl;
                            Module* pModule = cl.classFor(itMan->name()).create();
                            std::cout << pModule->name() << std::endl;
                            delete pModule;
                        }
                    }
                    cl.unloadLibrary(it->path());
                }
            } catch(Poco::LibraryLoadException& exc) {
                std::cout << exc.name() << std::endl;
            }
        }
    }



    RpcRegistry::instance().addMethod(new HealthRpcSupplier());
}
