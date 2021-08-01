//
// Created by Ivan Kishchenko on 01.08.2021.
//

#include "JoystickEvent.h"

namespace xbox {
    void to_json(nlohmann::json &j, const Xbox380Event &event) {
        j = nlohmann::json{
                {"rb",            event.rb},
                {"lb",            event.lb},
                {"rt",            event.rt},
                {"lt",            event.lt},
                {"btn-a",         event.btnA},
                {"btn-x",         event.btnX},
                {"btn-b",         event.btnB},
                {"btn-Y",         event.btnY},
                {"axis-lr-left",  event.axisLRLeft},
                {"axis-ud-left",  event.axisUDLeft},
                {"axis-lr-right", event.axisLRRight},
                {"axis-ud-right", event.axisUDRight},
                {"axis-lr-mid",   event.axisLRMid},
                {"axis-ud-mid",   event.axisUDMid},
        };
    }

    void from_json(const nlohmann::json &j, Xbox380Event &event) {
        j.at("rb").get_to(event.rb);
        j.at("lb").get_to(event.lb);

        j.at("rt").get_to(event.rt);
        j.at("lt").get_to(event.lt);

        j.at("btn-a").get_to(event.btnA);
        j.at("btn-x").get_to(event.btnX);
        j.at("btn-b").get_to(event.btnB);
        j.at("btn-y").get_to(event.btnY);
        j.at("axis-lr-left").get_to(event.axisLRLeft);
        j.at("axis-ud-left").get_to(event.axisUDLeft);
        j.at("axis-lt-right").get_to(event.axisLRRight);
        j.at("axis-ud-right").get_to(event.axisUDRight);
        j.at("axis-lr-mid").get_to(event.axisLRMid);
        j.at("axis-ud-mid").get_to(event.axisUDMid);
    }

}