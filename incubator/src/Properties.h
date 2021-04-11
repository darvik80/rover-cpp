//
// Created by Kishchenko, Ivan on 3/25/21.
//

#pragma once

#include <memory>
#include <vector>

struct Properties {
    typedef std::shared_ptr<Properties> Ptr;
    typedef std::vector<Ptr> VecPtr;

    virtual ~Properties() = default;
};
