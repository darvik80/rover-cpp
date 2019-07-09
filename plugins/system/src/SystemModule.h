//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-07.
//

#ifndef ROVER_SYSTEMMODULE_H
#define ROVER_SYSTEMMODULE_H

#include "plugin/Module.h"

class SystemModule : public Module {
    std::string name() const override;
};


#endif //ROVER_SYSTEMMODULE_H
