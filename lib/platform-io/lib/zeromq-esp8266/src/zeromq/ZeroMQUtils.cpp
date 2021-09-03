//
// Created by Ivan Kishchenko on 29.08.2021.
//

#include "ZeroMQUtils.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>


std::string byte2hex[256];
std::string hexpadding[16];
std::string byte2char[256];
std::string bytepadding[16];

void ZeroMQUtils::init() {
    char buf[16];
    for (int i = 0; i < 256; i++) {
        sprintf(buf, " %02x", i);
        byte2hex[i] = buf;
    }

    for (int i = 0; i < 16; i++) {
        std::string str;
        for (int j = 0; j < 16 - i; j++) {
            str += "   ";
        }
        hexpadding[i] = str;
    }

    for (int i = 0; i < 256; i++) {
        if (i <= 0x1f || i >= 0x7f) {
            byte2char[i] = ".";
        } else {
            byte2char[i] = (char) i;
        }
    }

    for (int i = 0; i < 16; i++) {
        std::string str = "";
        for (int j = 0; j < 16 - i; j++) {
            str += " ";
        }
        bytepadding[i] = str;
    }
}

std::string ZeroMQUtils::netDump(const uint8_t *data, std::size_t size) {
    if (!size) {
        return "";
    }

    std::stringstream str;

    str << std::endl;
    str << "         +-------------------------------------------------+" << std::endl;
    str << "         |  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |" << std::endl;
    str << "+--------+-------------------------------------------------+----------------+";

    std::size_t startIndex = 0, endIndex = size;
    std::size_t i = 0;
    for (i = 0; i < endIndex; i++) {
        auto relIdx = i - startIndex;
        auto relIdxMod16 = relIdx & 15;
        if (relIdxMod16 == 0) {
            str << std::endl << "|" << std::setw(8) << std::hex << relIdx << "|";
        }

        str << byte2hex[data[i]];
        if (relIdxMod16 == 15) {
            str << " |";
            for (size_t j = i - 15; j <= i; j++) {
                str << byte2char[data[j]];
            }
            str << "|";
        }
    }

    if (((i - startIndex) & 15) != 0) {
        auto remainder = size & 15;
        str << hexpadding[remainder];
        str << " |";
        for (size_t j = i - remainder; j < i; j++) {
            str << byte2char[data[j]];
        }
        str << bytepadding[remainder];
        str << "|";
    }
    str << std::endl << "+--------+-------------------------------------------------+----------------+";

    return str.str();
}
