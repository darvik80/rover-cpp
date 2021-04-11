//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include "Application.h"

using namespace std;


#include <iostream>
#include "serial/SerialPortManager.h"
#include <resource_files_manager.h>

using namespace resource_files;

int main(int argc, char *argv[]) {
    std::shared_ptr<ResourceFilesGetter> resource_manager =
            ResourceFilesGetter::getInstance();

    std::vector<std::string> file_names = resource_manager->getFileNames();

    FilePtr file_data_ptr{};

    for(const auto& elem : file_names){
        if(!resource_manager->getFilePtr(elem, file_data_ptr))
            std::cout << "Can`t find file: " << elem << std::endl;

        std::cout << "File " << elem << std::endl;
        std::cout << "_______________________________________\\n";
        while(file_data_ptr.start != file_data_ptr.end){ std::cout << *file_data_ptr.start++;}
        std::cout << "=======================================\\n";
    }

    SerialPortManager manager;
    auto ports = manager.listPorts();
    for (auto & port : ports) {
        std::cout << "Port: " << port.port << std::endl;
        std::cout << "Descr: " << port.description << std::endl;
        std::cout << "Id: " << port.hardwareId << std::endl;
        std::cout << "----" << std::endl;
    }

    Application app;
    app.run(argc, argv);

    return 0;
}
