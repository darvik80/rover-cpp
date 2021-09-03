//
// Created by Ivan Kishchenko on 02.09.2021.
//

#include "ZeroMQErrorCode.h"

const std::error_category & zeromq_category()
{
    static zeromq_error_category instance;
    return instance;
}

std::error_code make_error_code(zeromq_error e) {
    return {
            static_cast<int>(e),
            zeromq_category()
    };
}

std::error_condition make_condition(zeromq_error e) {
    return {
            static_cast<int>(e),
            zeromq_category()
    };
}
