#include <iostream>

#include "http/HttpServer.h"
#include "subsystem/Application.h"

int main(int argc, char *argv[]) {
    //HttpServer server;
    //server.run();

    Application app;
    return app.run(argc, argv);
}
