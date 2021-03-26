//
// Created by Kishchenko, Ivan on 3/24/21.
//

#ifndef ROVER_MODULE_H
#define ROVER_MODULE_H

#include <system_error>
#include <Config.h>

enum ModuleStatus {
    UP,
    OUT_OF_SERVICE,
    DOWN,
    UNKNOWN,
};

class Module {
public:
    typedef std::shared_ptr<Module> Ptr;

private:
    ModuleStatus _status{DOWN};
private:
    void setStatus(ModuleStatus status) {
        this->_status = status;
    }
public:
    virtual std::error_code create(Content& config) = 0;
    virtual std::error_code destroy() = 0;

public:
    virtual const char* getName() = 0;
    virtual ModuleStatus getStatus() = 0;

};


#endif //ROVER_MODULE_H
