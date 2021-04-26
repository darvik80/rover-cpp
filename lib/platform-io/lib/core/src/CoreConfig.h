//
// Created by Kishchenko, Ivan on 4/6/21.
//

#ifndef PLATFORM_IO_CORECONFIG_H
#define PLATFORM_IO_CORECONFIG_H

#ifdef ARDUINO_ARCH_AVR

#define ETL_NO_STL
#define ETL_NO_CPP_NAN_SUPPORT 1
#define ETL_NO_HUGE_VAL_SUPPORT

#include <etl/profiles/arduino_arm.h>

#include <Arduino.h>

#undef max
#undef min

#include <etl/string.h>
#include <etl/memory.h>
#include <etl/vector.h>

using string = etl::istring;

template<typename T>
using unique_ptr = etl::unique_ptr<T>;

template<typename T>
using vector = etl::ivector<T>;

#else

#if defined(ARDUINO)
#include <Arduino.h>
#endif

#include <string>
#include <memory>
#include <vector>

using string = std::string;

template<typename T>
using unique_ptr = std::unique_ptr<T>;

template<typename T>
using vector = std::vector<T>;

#endif

#endif //PLATFORM_IO_CORECONFIG_H
