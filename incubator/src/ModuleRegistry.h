//
// Created by Kishchenko, Ivan on 3/24/21.
//

#ifndef ROVER_MODULEREGISTRY_H
#define ROVER_MODULEREGISTRY_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <optional>
#include <exception>

#include <Module.h>

class ModuleRegistry {
private:
    typedef std::vector<Module::Ptr> ModuleVec;
    ModuleVec _modules;
public:
    virtual void registerModule(const Module::Ptr& module);

    virtual std::optional<ModuleStatus> getModuleStatus(std::string_view name);

    template<class C>
    C &getModule() {
        for (auto &ptr : _modules) {
            const Module *module(ptr.get());
            const C *pC = dynamic_cast<const C *>(module);
            if (pC) return *const_cast<C *>(pC);
        }
        throw std::invalid_argument(std::string("The subsystem has not been registered ") + typeid(C).name());
    }

    ModuleVec& getModules() {
        return _modules;
    }
};

#endif //ROVER_MODULEREGISTRY_H
