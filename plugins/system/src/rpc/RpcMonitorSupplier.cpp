//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "RpcMonitorSupplier.h"
#include "SysInfo.h"

#include <boost/version.hpp>
#include <boost/config.hpp>

SystemResponse RpcMonitorSupplier::exec() const {
    SystemResponse response;

    response.cpuCount = SysInfo::NumberOfProcessors();
    response.cpuTemp = SysInfo::cpuTemperature();
    response.physicalMemory = SysInfo::AmountOfPhysicalMemory();
    response.platform = BOOST_PLATFORM;

    return response;
}
