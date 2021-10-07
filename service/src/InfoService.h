//
// Created by Ivan Kishchenko on 26.04.2021.
//

#pragma once

#include "BaseService.h"
#include "net/http/JsonRpcHandler.h"

class InfoService : public BaseService, public em::TEventHandler<JsonRpcRegisterEvent>, public std::enable_shared_from_this<InfoService> {
public:
    void onEvent(const JsonRpcRegisterEvent &event) override;

    const char *name() override {
        return "info";
    }

    void postConstruct(Registry &registry) override;
};

