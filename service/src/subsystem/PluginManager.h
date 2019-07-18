//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#ifndef ROVER_PLUGINGMANAGER_H
#define ROVER_PLUGINGMANAGER_H

#include "Subsystem.h"
#include "plugin/Module.h"

#include <map>
#include <string>

class PluginManager : public Subsystem {
public:
    const char *name() const override;

    void postConstruct(Application &app) override;

    void preDestroy() override;
private:
    std::map<std::string, Module::Ptr> _modules;
};


#endif //ROVER_PLUGINGMANAGER_H
