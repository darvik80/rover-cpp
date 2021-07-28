//
// Created by Ivan Kishchenko on 26.04.2021.
//

#include <event/EventManagerService.h>
#include "InfoService.h"

BEGIN_DECLARE_DTO(HealthResponse)

__DECLARE_DTO_FIELDS1(std::string, status)
END_DECLARE_DTO

class HealthRpcSupplier : public RpcSupplier<HealthResponse> {
public:
    [[nodiscard]] std::string name() const override {
        return "health";
    }

    [[nodiscard]] HealthResponse exec() const override {
        HealthResponse response;
        response.status = "success";

        return response;
    }
};

void InfoService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
    registry.getService<EventManagerService>().subscribe<JsonRpcRegisterEvent>(shared_from_this());
}

void InfoService::onEvent(const JsonRpcRegisterEvent &event) {
    event.getJsonRpc().addMethod(std::make_shared<HealthRpcSupplier>());
}
