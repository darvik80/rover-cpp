//
// Created by Ivan Kishchenko on 26.06.2021.
//

#include "IRControllerService.h"
#include "IRControlMessage.h"

IRControllerService::IRControllerService(Registry &registry)
        : BaseService(registry) {}

void IRControllerService::postConstruct() {
    _remote = new HX1838IRRemote(11);
}

void IRControllerService::run() {
    auto code = _remote->decode();
    etl::send_message(getRegistry().getMessageBus(), ROUTER_APP, IRControlMessage(code));
}

void IRControllerService::preDestroy() {

}
