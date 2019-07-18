//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "RpcMonitorSupplier.h"
#include "SysInfo.h"

SystemResponse RpcMonitorSupplier::exec() const {
    SystemResponse response;

    response.cpu = SysInfo::NumberOfProcessors();
    response.physicalMemory = SysInfo::AmountOfPhysicalMemory();
    response.virtualMemory = SysInfo::AmountOfVirtualMemory();

    return response;
}
