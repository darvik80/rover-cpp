#include <iostream>

#include "http/HttpServer.h"
#include "subsystem/Application.h"
#include "MainApplication.h"

int main(int argc, char *argv[]) {
    MainApplication app;
    return app.run(argc, argv);
}
