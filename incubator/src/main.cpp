//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include "Application.h"

using namespace std;


#include <iostream>
#include <resources/ResourceManager.h>

int main(int argc, char *argv[]) {

    for (const auto& item : ResourceManager::instance().getResourceNames()) {
        std::cout << item << std::endl;
    }

    auto resource = ResourceManager::instance().getResource("settings.json");
    std::string data((const char*)resource->getOffset(), resource->getSize());
    std::cout << data << std::endl;

    Application app;
    app.run(argc, argv);

    return 0;
}
