//
// Created by Ivan Kishchenko on 15.07.2021.
//

#ifdef SUPPORT_BARCODE

#include "BarcodeReaderService.h"

char key[KEY_UNKNOWN] = { 0 };

void initKey() {
    key[KEY_MINUS] = '-';
    key[KEY_0] = '0';
    key[KEY_1] = '1';
    key[KEY_2] = '2';
    key[KEY_3] = '3';
    key[KEY_4] = '4';
    key[KEY_5] = '5';
    key[KEY_6] = '6';
    key[KEY_7] = '7';
    key[KEY_8] = '8';
    key[KEY_9] = '9';

    key[KEY_Q] = 'Q';
    key[KEY_W] = 'W';
    key[KEY_E] = 'E';
    key[KEY_R] = 'R';
    key[KEY_T] = 'T';
    key[KEY_Y] = 'Y';
    key[KEY_U] = 'U';
    key[KEY_I] = 'I';
    key[KEY_O] = 'O';
    key[KEY_P] = 'P';

    key[KEY_A] = 'A';
    key[KEY_S] = 'S';
    key[KEY_D] = 'D';
    key[KEY_F] = 'F';
    key[KEY_G] = 'G';
    key[KEY_H] = 'H';
    key[KEY_J] = 'J';
    key[KEY_K] = 'K';
    key[KEY_L] = 'L';

    key[KEY_Z] = 'Z';
    key[KEY_X] = 'X';
    key[KEY_C] = 'C';
    key[KEY_V] = 'V';
    key[KEY_B] = 'B';
    key[KEY_N] = 'N';
    key[KEY_M] = 'M';
}


void BarcodeReaderService::postConstruct(Registry &registry) {
    initKey();
    BaseService::postConstruct(registry);

    int fd;
    char name[256] = "Unknown";
    const char *device = "/dev/input/by-id/usb-USB_Adapter_USB_Device-event-kbd";

    //Open Device
    if ((fd = open(device, O_RDONLY)) == -1) {
        warning("{} is not a valid device", device);
        return;
    }

    //Print Device Name
    ioctl(fd, EVIOCGNAME (sizeof(name)), name);
    info("Reading From : {} ({})", device, name);

    _stream = std::make_unique<boost::asio::posix::stream_descriptor>(
            registry.getIoService()
    );
    _stream->assign(fd);
    _stream->async_read_some(
            boost::asio::buffer(&_events, sizeof(_events)),
            [this](const boost::system::error_code error, std::size_t readable) {
                onRead(error, readable);
            }
    );

}

void BarcodeReaderService::onRead(const boost::system::error_code &error, std::size_t readable) {
    if (!error) {
        std::size_t size = readable / sizeof(input_event);
        for (int idx = 0; idx < size; idx++) {
            if (_events[idx].value == 0) {
                continue;
            }
            if (_events[idx].type == EV_KEY && _events[idx].value == 1) {
                switch (_events[idx].code) {
                    case KEY_LEFTSHIFT:
                        continue;
                    case KEY_ENTER:
                        info("result: {}", _barcode);
                        _barcode = "";
                        continue;
                    default:
                        _barcode += key[_events[idx].code];
                        break;
                }
            }
        }

        _stream->async_read_some(
                boost::asio::buffer(&_events, sizeof(_events)),
                [this](const boost::system::error_code error, std::size_t readable) {
                    onRead(error, readable);
                }
        );
    } else {
        warning("read failed: {}", error.message());
    }
}

#endif