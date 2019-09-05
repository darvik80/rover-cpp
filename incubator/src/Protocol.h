//
// Created by Ivan Kishchenko on 05/09/2019.
//

#ifndef ROVER_PROTOCOL_H
#define ROVER_PROTOCOL_H

// { Commands
#include <cstdint>
#include <string>
#include <sstream>

#define INIT    0x47534d3f  //  "?INI"
#define MSG     0x494e493f  //  "?MSG"
#define SET     0x5445533f  //  "?SET
#define GET     0x5445473f  //  "?GET"
#define EVENT   0x5456453f  //  "?EVT"
#define MODE    0x444f4d3f  //  "?MOD"
#define PUT     0x5455503f  //  "?PUT"
// } Commands

class Operation {
private:
    uint32_t _operation;
public:
    Operation(uint32_t operation) : _operation(operation) {}

    std::string getOperation() {
        return std::string((char*)&_operation, 4);
    }

    virtual std::string toString() = 0;
    virtual void fromString(const std::string& str) = 0;
};

class Handshake : public Operation {
};

class Message : public Operation {
private:
    std::string _context;
public:
    const std::string &getContext() const {
        return _context;
    }
};

class SetPin : public Operation {
private:
    int _pin;
    int _value;
public:
    int getPin() const {
        return _pin;
    }

    int getValue() const {
        return _value;
    }

    void setPin(int pin) {
        _pin = pin;
    }

    void setValue(int value) {
        _value = value;
    }
};

class GetPin : public Operation {
private:
    int _pin;
    int _value;
public:
    GetPin(int pin, int value) : Operation(GET), _pin(pin), _value(value) {}

    int getPin() const {
        return _pin;
    }

    int getValue() const {
        return _value;
    }

    void setPin(int pin) {
        _pin = pin;
    }

    void setValue(int value) {
        _value = value;
    }

    std::string toString() override {
        std::stringstream str;
        str << getOperation() << " " << getPin() << ":" << getValue();
        return str.str();
    }

    void fromString(const std::string& str) override {
        if (str.size() < 4) {
            throw ex
        }

    }
};


#endif //ROVER_PROTOCOL_H
