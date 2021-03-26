//
// Created by Kishchenko, Ivan on 3/24/21.
//

#include "ModuleRegistry.h"

void ModuleRegistry::registerModule(const Module::Ptr& module) {
    _modules.emplace_back(module);
}

std::optional<ModuleStatus> ModuleRegistry::getModuleStatus(std::string_view name) {
    auto iter = std::find_if(_modules.begin(), _modules.end(), [name](const auto module) { return module->getName() == name; });

    if (iter != _modules.end()) {
        return iter->get()->getStatus();
    }

    return {};
}
