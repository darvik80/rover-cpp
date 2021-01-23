//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "RpcMonitorSupplier.h"
#include "SysInfo.h"

#include <boost/version.hpp>
#include <boost/config.hpp>

#include <iostream>
#include <sys/utsname.h>

SystemResponse RpcMonitorSupplier::exec() const {
    struct utsname uNameData;

    uname(&uNameData);

    SystemResponse response;

    response.cpuCount = SysInfo::NumberOfProcessors();
    response.cpuTemp = SysInfo::cpuTemperature();
    response.batteryTemp = SysInfo::batteryTemperature();
    response.physicalMemory = SysInfo::AmountOfPhysicalMemory();
    response.platform = BOOST_PLATFORM;
    response.osName = uNameData.version;

    return response;
}
