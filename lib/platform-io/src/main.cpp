#include "CoreConfig.h"
#include "Application.h"

Application app;

void setup() {
    app.postConstruct();
}

void loop() {
    app.run();
}