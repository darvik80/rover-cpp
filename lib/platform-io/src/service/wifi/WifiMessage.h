//
// Created by Ivan Kishchenko on 10.07.2021.
//

#ifndef PLATFORM_IO_WIFIMESSAGE_H
#define PLATFORM_IO_WIFIMESSAGE_H

#include "service/Const.h"

class WifiMessageConnected : public etl::message<MSG_WIFI_CONNECTED> {
};

class WifiMessageDisconnected : public etl::message<MSG_WIFI_DISCONNECTED> {
};

#endif //PLATFORM_IO_WIFIMESSAGE_H
