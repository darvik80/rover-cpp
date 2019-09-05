//
// Created by Ivan Kishchenko on 05/09/2019.
//

#ifndef ROVER_PROTOCOL_H
#define ROVER_PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <boost/lexical_cast.hpp>

// { Commands
#define PROTO_INIT      0x494e493f  //  "?INI"
#define PROTO_MSG       0x47534d3f  //  "?MSG"
#define PROTO_SET       0x5445533f  //  "?SET
#define PROTO_GET       0x5445473f  //  "?GET"
#define PROTO_EVENT     0x5456453f  //  "?EVT"
#define PROTO_MODE      0x444f4d3f  //  "?MOD"
#define PROTO_PUT       0x5455503f  //  "?PUT"
// } Commands

namespace protocol {

    class Operation {
    private:
        uint32_t _operation;
    public:
        typedef std::shared_ptr<Operation> Ptr;

        explicit Operation(uint32_t operation) : _operation(operation) {}

        std::string getOperation() {
            return std::string((char *) &_operation, 4);
        }

        virtual std::string toString() = 0;
    };

    class Handshake : public Operation {
    public:
        Handshake() : Operation(PROTO_INIT) {}

        std::string toString() override {
            return getOperation();
        }
    };

    class Message : public Operation {
    private:
        std::string _context;
    public:
        explicit Message(const std::string &context) : Operation(PROTO_MSG), _context(context) {}

        const std::string &getContext() const {
            return _context;
        }

        std::string toString() override {
            return getOperation() + " " + _context;
        }
    };

    class SetPin : public Operation {
    private:
        int _pin;
        int _value;
    public:
        SetPin(int pin, int value) : Operation(PROTO_SET), _pin(pin), _value(value) {}

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
    };

    class PutPin : public Operation {
    private:
        int _pin;
        bool _value;
    public:
        PutPin(int pin, bool value) : Operation(PROTO_PUT), _pin(pin), _value(value) {}

        int getPin() const {
            return _pin;
        }

        int getValue() const {
            return _value;
        }

        void setPin(int pin) {
            _pin = pin;
        }

        void setValue(bool value) {
            _value = value;
        }

        std::string toString() override {
            std::stringstream str;
            str << getOperation() << " " << getPin() << ":" << getValue();
            return str.str();
        }
    };

    class GetPin : public Operation {
    private:
        int _pin;
    public:
        explicit GetPin(int pin) : Operation(PROTO_GET), _pin(pin) {}

        int getPin() const {
            return _pin;
        }

        void setPin(int pin) {
            _pin = pin;
        }

        std::string toString() override {
            return getOperation() + " " + boost::lexical_cast<std::string>(getPin());
        }
    };

    class ModePin : public Operation {
    private:
        int _pin;
        int _value;
    public:
        ModePin(int pin, int value) : Operation(PROTO_MODE), _pin(pin), _value(value) {}

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
    };


    class OperationManager {
    public:
        Operation::Ptr parse(const std::string &packet);
    };
}

#endif //ROVER_PROTOCOL_H
