//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#ifndef ROVER_SYSINFO_H
#define ROVER_SYSINFO_H


#include <cstdint>

#define V8_OS_MACOSX

class SysInfo {
public:
    static int NumberOfProcessors();
    static int64_t AmountOfPhysicalMemory();
    static int64_t AmountOfVirtualMemory();
};


#endif //ROVER_SYSINFO_H
