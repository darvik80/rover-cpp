//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "RpcMonitorSupplier.h"
#include "SysInfo.h"

SystemResponse RpcMonitorSupplier::exec() const {
    SystemResponse response;

    response.cpuCount = SysInfo::NumberOfProcessors();
    response.cpuTemp = SysInfo::cpuTemperature();
    response.physicalMemory = SysInfo::AmountOfPhysicalMemory();

    return response;
}
