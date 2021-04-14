//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include "Application.h"

using namespace std;


#include <iostream>
#include "serial/SerialPortManager.h"

#include <CoreFoundation/CFBundle.h>

std::string get_resources_dir() {

    CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
    char resourcePath[PATH_MAX];

    if (CFURLGetFileSystemRepresentation(resourceURL, true,
                                         (UInt8 *) resourcePath,
                                         PATH_MAX)) {
        if (resourceURL != nullptr) {
            CFRelease(resourceURL);
        }

        return resourcePath;
    }

    return std::string();
}

#include <fstream>

int main(int argc, char *argv[]) {
    std:ifstream file(get_resources_dir() + "/settings.json");
    std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    std::cout << str << std::endl;

    SerialPortManager manager;
    auto ports = manager.listPorts();
    for (auto &port : ports) {
        std::cout << "Port: " << port.port << std::endl;
        std::cout << "Descr: " << port.description << std::endl;
        std::cout << "Id: " << port.hardwareId << std::endl;
        std::cout << "----" << std::endl;
    }

    Application app;
    app.run(argc, argv);

    return 0;
}
