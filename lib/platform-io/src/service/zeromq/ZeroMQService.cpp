//
// Created by Ivan Kishchenko on 29.08.2021.
//

#include "ZeroMQService.h"
#include <zeromq/ZeroMQUtils.h>

ZeroMQService::ZeroMQService(Registry &registry)
        : BaseService(registry), message_router(ZERO_MQ_SERVICE) {
    ZeroMQUtils::init();
}

void ZeroMQService::postConstruct() {
    getRegistry().getMessageBus().subscribe(*this);
}

void ZeroMQService::on_receive(const WifiMessageConnected &msg) {
    _server.start(5556);
}

void ZeroMQService::on_receive(const WifiMessageDisconnected &msg) {
    _server.shutdown();
}
