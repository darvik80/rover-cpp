//
// Created by Ivan Kishchenko on 10.07.2021.
//

#ifndef PLATFORM_IO_WIFIMESSAGE_H
#define PLATFORM_IO_WIFIMESSAGE_H

#ifdef SUPPORT_ETHERNET

#include "service/Const.h"

class WifiMessageConnected : public etl::message<MSG_WIFI_CONNECTED> {
};

class WifiMessageDisconnected : public etl::message<MSG_WIFI_DISCONNECTED> {
};

#endif

#endif //PLATFORM_IO_WIFIMESSAGE_H
