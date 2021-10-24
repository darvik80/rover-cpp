//
// Created by Ivan Kishchenko on 29.08.2021.
//

#pragma once

#ifdef ESP8266

//#include <zeromq/esp8266/ZeroMQServer.h>
//#include <zeromq/ZeroMQUtils.h>

#include <network/esp8266/AsyncTcpServer.h>

#include "service/Service.h"
#include "service/wifi/WifiMessage.h"

class ZeroMQService : public BaseService, public etl::message_router<ZeroMQService, WifiMessageConnected, WifiMessageDisconnected, WifiClientMessageClientDisconnected> {
    network::AsyncTcpServer* _server;
public:
    explicit ZeroMQService(Registry &registry);

    void postConstruct() override;

    void on_receive(const WifiMessageConnected &msg);
    void on_receive(const WifiMessageDisconnected &msg);
    void on_receive(const WifiClientMessageClientDisconnected &msg);

    void on_receive_unknown(const etl::imessage &msg) {}
};

#endif