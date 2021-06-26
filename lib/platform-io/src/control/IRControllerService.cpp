//
// Created by Ivan Kishchenko on 26.06.2021.
//

#include "IRControllerService.h"
#include "IRControlMessage.h"

IRControllerService::IRControllerService(Registry &registry, IRRemote* remote)
        : BaseService(registry), _remote(remote) {}

void IRControllerService::run() {
    auto code = _remote->decode();
    etl::send_message(getRegistry().getMessageBus(),IRControlMessage(code));
}
