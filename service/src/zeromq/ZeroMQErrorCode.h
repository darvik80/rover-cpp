//
// Created by Ivan Kishchenko on 02.09.2021.
//

#pragma once

#include <system_error>

enum class zeromq_error {
    invalid_format,
    socket_type_not_support,
};


class zeromq_error_category :
        public std::error_category {
public:
    [[nodiscard]] const char *name() const _NOEXCEPT override {
        return "zeromq";
    }

    [[nodiscard]] std::string message(int ev) const override {
        switch (static_cast<zeromq_error>(ev)) {
            case zeromq_error::invalid_format:
                return "invalid message format";
            case zeromq_error::socket_type_not_support:
                return "Socket type not supported";
            default:
                return "Unknown HTTP error";
        }
    }

    [[nodiscard]] std::error_condition default_error_condition(int ev) const _NOEXCEPT override {
        switch(static_cast<zeromq_error>(ev)) {
            case zeromq_error::invalid_format:
                return std::errc::invalid_argument;
            default:
                return std::error_condition(ev, *this);
        }
    }
};

const std::error_category &zeromq_category();

std::error_code make_error_code(zeromq_error e);

std::error_condition make_condition(zeromq_error e);

namespace std
{
    template<>
    struct is_error_code_enum<zeromq_error>:
            public true_type {};
}
