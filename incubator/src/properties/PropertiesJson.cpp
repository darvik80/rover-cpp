//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include "PropertiesJson.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/dll.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

void PropertiesJson::load(const std::string &filePath) {
    property_tree::read_json(filePath, this->tree);
}

std::string PropertiesJson::getString(const std::string &name) const {
    return tree.get<string>(name);
}

bool PropertiesJson::getBoolean(const std::string &name) const {
    return tree.get<bool>(name);
}

int PropertiesJson::getInt(const std::string &name) const {
    return tree.get<int>(name);
}

long PropertiesJson::getLong(const std::string &name) const {
    return tree.get<long>(name);
}

double PropertiesJson::getDouble(const std::string &name) const {
    return tree.get<double>(name);
}

std::list<std::string> PropertiesJson::getStringArray(const std::string &name) const {
    std::list<std::string> result;
    // Iterator over all animals
    for (const auto &item : tree.get_child(name)) {
        result.push_back(item.second.get_value<string>());
    }

    return result;
}

