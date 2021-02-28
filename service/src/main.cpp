#include "MainApplication.h"

#ifdef PROFILE_GPIO
#include <wiringPi.h>
#include <wiringPiI2C.h>

// Define some device parameters
#define I2C_ADDR   0x21 // I2C device address

// Define some device constants
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line

#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off

#define ENABLE  0b00000100 // Enable bit

#endif

#include <unistd.h>

int main(int argc, char *argv[]) {
#ifdef PROFILE_GPIO
    if (wiringPiSetup() == -1) {
        exit(1);
    }

    int handler = wiringPiI2CSetupInterface("/dev/i2c-1", I2C_ADDR);
    if (handler < 0) {
        exit(handler);
    }

    for (int i = 0; i < 1000; i++) {
        sleep(1);
        wiringPiI2CWrite(handler, 64);
    }
#endif

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

    MainApplication app;
    return app.run(argc, argv);
}
