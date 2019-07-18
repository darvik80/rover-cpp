//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_APPLICATION_H
#define ROVER_APPLICATION_H

#include <vector>
#include <memory>
#include <string>

#include "Subsystem.h"
#include "exception/LogicException.h"
#include "properties/Properties.h"

class Logger;

class Application : public Subsystem {
public:
    Application();

    const char *name() const override;

    void postConstruct(Application &app) override;

    virtual int run(int argc, char *argv[]);
    virtual void run();

    void preDestroy() override;

    Logger& logger();

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
    typedef std::vector<Subsystem::Ptr> SubSystemVec;
    SubSystemVec _spSubsystems;

    SPProperties _properties;

    std::shared_ptr<Logger> _logger;
};


#endif //ROVER_APPLICATION_H