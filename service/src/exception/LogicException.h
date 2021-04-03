//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_LOGICEXCEPTION_H
#define ROVER_LOGICEXCEPTION_H

#include <stdexcept>

class NotImplementedException : public std::logic_error {
public:
    explicit NotImplementedException(const std::string &string) : logic_error(string) {}

    explicit NotImplementedException(const char *string) : logic_error(string) {}
};

class ForbiddenException : public std::logic_error {
public:
    explicit ForbiddenException(const std::string &string) : logic_error(string) {}

    explicit ForbiddenException(const char *string) : logic_error(string) {}

};

class NotFoundException : public std::logic_error {
public:
    explicit NotFoundException(const std::string &string) : logic_error(string) {}

    explicit NotFoundException(const char *string) : logic_error(string) {}
};

#endif //ROVER_LOGICEXCEPTION_H
