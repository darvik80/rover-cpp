//
// Created by Ivan Kishchenko on 29.08.2021.
//

#pragma once
#include <boost/asio.hpp>
#include "ZeroMQProtocol.h"
#include "ZeroMQBuf.h"
#include "scheduler/Scheduler.h"

#include <string>
#include <unordered_set>

enum class ZeroMQStatus {
    ZMQ_Idle,
    ZMQ_C_Wait_Header,
    ZMQ_C_Wait_Greeting,
    ZMQ_C_Wait_Ready,
    ZMQ_Stream,
};

class ZeroMQSocket {
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::socket _socket;


    std::array<char, 256> _incBuf;
    ZeroMQBufFix<1024> _inc;

    ZeroMQStatus _status{ZeroMQStatus::ZMQ_Idle};
    TimerHandler _timer;

    std::unordered_set<std::string> _topics;
public:
    ZeroMQSocket(boost::asio::io_service& service, std::string_view address, int port);
    void onIdle();
    void publish(std::string_view topic, std::string_view data);
private:
    void startConnect();
    void startRecv();
    void startSend(std::unique_ptr<ZeroMQCharBuf>& buf);

    void handleData(ZeroMQBuf<>& inc);

    void onCommand(const ZeroMQCommand& cmd);
    void onMessage(const ZeroMQMessage& cmd);
};
