//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_HX1838IRREMOTE_H
#define PLATFORM_IO_HX1838IRREMOTE_H

#ifdef HX1838_IR_REMOTE

#ifndef IR_REMOTE
#define IR_REMOTE
#endif

#include "device/controller/IRRemote.h"
#include <IRremote.h>

class HX1838IRRemote : public IRRemote {
    IRrecv _receiver;
    decode_results _lastDecodeResult{};
private:
    static Button code2btn(unsigned long code) {
        switch (code) {
            case 0xFF629D:
                return BTN_UP;
            case 0xFFA857:
                return BTN_DOWN;
            case 0xFF22DD:
                return BTN_LEFT;
            case 0xFFC23D:
                return BTN_RIGHT;
            case 0xFF02FD:
                return BTN_ENTER;
            case 0xFF42BD:
                return BTN_START;
            case 0xFF52AD:
                return BTN_HASHTAG;
            case 0xFF6897:
                return BTN_1;
            case 0xFF9867:
                return BTN_2;
            case 0xFFB04F:
                return BTN_3;
            case 0xFF30CF:
                return BTN_4;
            case 0xFF18E7:
                return BTN_5;
            case 0xFF7A85:
                return BTN_6;
            case 0xFF10EF:
                return BTN_7;
            case 0xFF38C7:
                return BTN_8;
            case 0xFF5AA5:
                return BTN_9;
            case 0xFF4AB5:
                return BTN_0;
            default:
                break;
        }

        return BTN_NONE;
    }
public:
    explicit HX1838IRRemote(int pin) : _receiver(pin) {
        _receiver.enableIRIn();
    }

    const char* name() override {
        return "hx1838";
    }

    Button decode() override {
        decode_results results{};
        if (_receiver.decode( &results )) {
            if (results.value == 0xFFFFFFFF) {
                results = _lastDecodeResult;
            }
            if (results.value != 0xFFFFFFFF) {
                _lastDecodeResult = results;
            }

            _receiver.resume();

            return code2btn(results.value);
        }

        return BTN_NONE;
    }
};

#endif

#endif //PLATFORM_IO_HX1838IRREMOTE_H
