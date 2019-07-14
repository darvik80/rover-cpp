//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_SUBSYSTEM_H
#define ROVER_SUBSYSTEM_H

class Application;

class Subsystem {
public:
    virtual const char * name() const = 0;
    virtual void postConstruct(Application& app) = 0;
    virtual void preDestroy() = 0;

    virtual ~Subsystem() = default;
};

#endif //ROVER_SUBSYSTEM_H
