//
// Created by Ivan Kishchenko on 01.08.2021.
//

#pragma once

#include <event/Event.h>
#include <nlohmann/json.hpp>

namespace xbox {

    class JoystickEvent : public em::Event {
    };

    class Xbox380Event : public JoystickEvent {
    public:
        int axisLRLeft{0};
        int axisUDLeft{0};

        int axisLRRight{0};
        int axisUDRight{0};

        int axisLRMid{0};
        int axisUDMid{0};

        bool rb{false};
        bool lb{false};

        int lt{0};
        int rt{0};

        bool btnA{false};
        bool btnX{false};
        bool btnB{false};
        bool btnY{false};
    };

    void to_json(nlohmann::json &j, const Xbox380Event &event);

    void from_json(const nlohmann::json &j, Xbox380Event &event);

}