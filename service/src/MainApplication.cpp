//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-18.
//

#include "MainApplication.h"
#include "http/HttpServer.h"
#include "const.h"
#include "subsystem/PluginManager.h"

void MainApplication::postConstruct(Application &app) {
    Application::postConstruct(app);
}

void MainApplication::run() {
    RpcMethod::PtrVec methods;
    auto& manager = getSubsystem<PluginManager>();
    for (const auto& module : manager.getModules()) {
        for (const auto& rpc : module->getRpcMethods()) {
            methods.push_back(rpc.second);
        }
    }

    HttpServer server(getProperties()->getString(PROP_APPLICATION_DATA_DIR) + "www");
    server.run(methods);
}

int MainApplication::run(int argc, char **argv) {
    return Application::run(argc, argv);
}

void MainApplication::preDestroy() {
    Application::preDestroy();
}
