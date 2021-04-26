//
// Created by Ivan Kishchenko on 24.04.2021.
//

#ifndef ROVER_LOGGINGPROPERTIES_H
#define ROVER_LOGGINGPROPERTIES_H


struct LoggingProperties : Properties {
    std::string level{"debug"};
    bool console{false};
    bool file{false};
};


#endif //ROVER_LOGGINGPROPERTIES_H
