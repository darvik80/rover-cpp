//
// Created by Ivan Kishchenko on 2019-07-11.
//


#include "subsystem/Application.h"
#include "http/HttpServer.h"

int main(int argc, char *argv[]) {
    HttpServer server;
    server.run();

    Application app;
    return app.run(argc, argv);
}
