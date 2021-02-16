#include "MainApplication.h"

int main(int argc, char *argv[]) {
//    std::shared_ptr<ResourceFilesGetter> resource_manager =
//            ResourceFilesGetter::getInstance();
//
//    std::vector<std::string> file_names = resource_manager->getFileNames();
//
//    FilePtr file_data_ptr;
//
//    for(const auto& elem : file_names){
//        if(!resource_manager->getFilePtr(elem, file_data_ptr))
//            std::cout << "Can`t find file: " << elem << std::endl;
//
//        std::cout << "File " << elem << std::endl;
//        std::cout << "_______________________________________\\n";
//        while(file_data_ptr.start != file_data_ptr.end){ std::cout << *file_data_ptr.start++;}
//        std::cout << "=======================================\\n";
//    }
//    return 0;

    MainApplication app;
    return app.run(argc, argv);
}
