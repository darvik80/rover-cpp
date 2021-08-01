//
// Created by Ivan Kishchenko on 19.05.2021.
//

#ifndef ROVER_SERIALEVENT_H
#define ROVER_SERIALEVENT_H

#include <event/Event.h>

#include <utility>

class SerialEvent : public em::Event {
private:
    uint8_t _msgId;
    std::vector<uint8_t> _data;
public:
    SerialEvent(Service::Ptr source, uint8_t msgId, const uint8_t *data, size_t size)
            : Event(std::move(source)), _data(size), _msgId(msgId) {
        std::copy(data, data + size, _data.begin());

    }

    [[nodiscard]] uint8_t getMsgId() const {
        return _msgId;
    }

    [[nodiscard]] const std::vector<uint8_t> &getData() const {
        return _data;
    }
};


#endif //ROVER_SERIALEVENT_H
