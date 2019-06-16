//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "HealthRpcSupplier.h"

HealthResponse HealthRpcSupplier::exec() const {
    HealthResponse response;
    response.status = "success";

    return response;
}
