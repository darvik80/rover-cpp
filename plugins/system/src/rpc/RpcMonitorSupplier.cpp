//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "RpcMonitorSupplier.h"
#include "SysInfo.h"

#include <boost/version.hpp>
#include <boost/config.hpp>

#include <iostream>

SystemResponse RpcMonitorSupplier::exec() const {
    SystemResponse response;

    response.cpuCount = SysInfo::NumberOfProcessors();
    response.cpuTemp = SysInfo::cpuTemperature();
    response.batteryTemp = SysInfo::batteryTemperature();
    response.physicalMemory = SysInfo::AmountOfPhysicalMemory();
    response.platform = BOOST_PLATFORM;

    auto t = response.marshal().dump();
    std::cout << t << std::endl;
    return response;
}
