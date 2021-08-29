//
// Created by Ivan Kishchenko on 29.08.2021.
//

#ifndef ROVER_ZEROMQSOCKET_H
#define ROVER_ZEROMQSOCKET_H

#include <boost/asio.hpp>
#include "ZeroMQProtocol.h"
#include "scheduler/Scheduler.h"

enum ZeroMQStatus {
    ZMQ_Idle,
    ZMQ_Greeting,
    ZMQ_Ready,
    ZMQ_Stream,
};

class ZeroMQSocket {
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::socket _socket;
    std::array<char, 1024> _incBuf;

    ZeroMQStatus _status{ZMQ_Idle};
    TimerHandler _timer;
public:
    ZeroMQSocket(boost::asio::io_service& service, std::string_view address, int port);
    void onIdle();
private:
    void startConnect();
    void startRecv();
    void startSend(std::unique_ptr<boost::asio::streambuf>& buf);
};


#endif //ROVER_ZEROMQSOCKET_H
