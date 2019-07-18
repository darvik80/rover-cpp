//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#ifndef ROVER_RPCMONITORSUPPLIER_H
#define ROVER_RPCMONITORSUPPLIER_H

#include <rpc/json/Helper.h>
#include "rpc/RpcMethod.h"

#include <cstdint>

BEGIN_DECLARE_DTO(SystemResponse)

__DECLARE_DTO_FIELDS3(
        int, cpuCount,
        float, cpuTemp,
        int64_t, physicalMemory
)
END_DECLARE_DTO


class RpcMonitorSupplier : public RpcSupplier<SystemResponse> {
public:
    std::string name() const override {
        return "systemMonitor";
    }

private:
    SystemResponse exec() const override;
};


#endif //ROVER_RPCMONITORSUPPLIER_H