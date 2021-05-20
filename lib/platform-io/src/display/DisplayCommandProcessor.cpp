//
// Created by Kishchenko, Ivan on 5/20/21.
//

#include "DisplayCommandProcessor.h"

void DisplayCommandProcessor::process(const char *data, size_t size) {
    if (!size) {
        return;
    }

    auto cmd = data[0];

}
