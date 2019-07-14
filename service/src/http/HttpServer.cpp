//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include "HttpServer.h"
#include "HttpWorker.h"

#include "rpc/HealthRpcSupplier.h"

#include <iostream>

using namespace std;

#include "JsonRpcHandler.h"


int HttpServer::run() {
    JsonRpcHandler::Ptr rpcHandler = std::make_shared<JsonRpcHandler>();
    rpcHandler->addMethod(std::make_shared<HealthRpcSupplier>());

    try {
        auto const address = boost::asio::ip::make_address("0.0.0.0");
        unsigned short port = 8080;
        std::string doc_root = "/Users/darvik/Projects/cpp/rover/cmake-build-debug/var/www";
        int num_workers = 1;
        //bool spin = (std::strcmp(argv[5], "spin") == 0);

        boost::asio::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};

        std::list<HttpWorker> workers;
        for (int i = 0; i < num_workers; ++i) {
            workers.emplace_back(rpcHandler, acceptor, doc_root);
            workers.back().start();
        }

        ioc.run();
        /*
        if (spin) {
            for (;;) ioc.poll();
        } else {
            ioc.run();
        }
         */

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}