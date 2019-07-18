#include <iostream>

#include "http/HttpServer.h"
#include "subsystem/Application.h"
#include "MainApplication.h"

#include <IOKit/IOKitLib.h>

int main(int argc, char *argv[]) {



    MainApplication app;
    return app.run(argc, argv);
}
