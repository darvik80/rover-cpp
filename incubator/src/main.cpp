//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include <Kernel.h>

using namespace std;

Kernel kernel;

void setup() {
   kernel.create();
}

void loop() {
    kernel.run();
}

int main(int argc, char *argv[]) {
    setup();

    while(kernel.run()) { };

    return 0;
}
