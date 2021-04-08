//
// Created by Kishchenko, Ivan on 4/6/21.
//

#ifndef PLATFORM_IO_CORECONFIG_H
#define PLATFORM_IO_CORECONFIG_H

#ifdef ARDUINO_ARCH_AVR

#include <etl/string.h>

using string = etl::string;
#else

#include <string>

using string = std::string;
#endif

#endif //PLATFORM_IO_CORECONFIG_H
