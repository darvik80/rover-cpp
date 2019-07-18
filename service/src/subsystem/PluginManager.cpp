//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "PluginManager.h"

#include <boost/dll/import.hpp> // for import_alias
#include <boost/dll/library_info.hpp>
#include <boost/function.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "Application.h"
#include "subsystem/Logger.h"
#include "../const.h"

using namespace boost;

const char *PluginManager::name() const {
    return "pluginManager";
}

void PluginManager::postConstruct(Application &app) {
    std::string path = app.getProperties()->getString(PROP_APPLICATION_LIBRARY);

    typedef Module::Ptr (pluginapi_create_t)();
    boost::function<pluginapi_create_t> creator;

    filesystem::directory_iterator end_iter;
    for (filesystem::directory_iterator it(path); it != end_iter; ++it) {
        app.logger().info(it->path().string());

        //if(filesystem::is_regular_file(*it) && it->path().extension() == "dylib") ret.push_back(it->path().filename());

        dll::shared_library lib(it->path());//, dll::load_mode::append_decorations);
        if (!lib.has("createModule")) {
            // no such symbol
            continue;
        }
        creator = boost::dll::import_alias<pluginapi_create_t>(path + "libsystem", "createModule", dll::load_mode::append_decorations);

        std::cout << creator()->name() << std::endl;
        std::cout << creator()->name() << std::endl;
    }
}

void PluginManager::preDestroy() {

}
