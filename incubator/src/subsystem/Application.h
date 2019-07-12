//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_APPLICATION_H
#define ROVER_APPLICATION_H


#include "Subsystem.h"
#include <vector>
#include <memory>
#include <string>
#include <exception/LogicException.h>
#include <properties/Properties.h>

class Logger;

class Application : public Subsystem{
protected:
    const char *name() const override;

    void postConstruct(Application &app) override;

    virtual void run();

    void preDestroy() override;

    Logger& logger();

public:
    Application();

    int run(int argc, char *argv[]);

    void addSubsystem(std::shared_ptr<Subsystem> spSubSystem) {
        _spSubsystems.push_back(spSubSystem);
    }

    template<class C> C& getSubsystem() {
        for (auto & _spSubsystem : _spSubsystems) {
            const Subsystem* pSS(_spSubsystem.get());
            const C* pC = dynamic_cast<const C*>(pSS);
            if (pC) return *const_cast<C*>(pC);
        }
        throw NotFoundException(std::string("The subsystem has not been registered ") + typeid(C).name());
    }

    SPProperties getProperties() {
        return _properties;
    }

    ~Application() override;

private:
    typedef std::vector<std::shared_ptr<Subsystem> > SubSystemVec;
    SubSystemVec _spSubsystems;

    SPProperties _properties;

    std::shared_ptr<Logger> _logger;
};


#endif //ROVER_APPLICATION_H
