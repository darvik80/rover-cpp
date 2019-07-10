//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_MODULE_H
#define ROVER_MODULE_H

#include <string>

class Module {
public:

    virtual std::string name() const = 0;

    virtual ~Module() = default;
};


#endif //ROVER_MODULE_H
