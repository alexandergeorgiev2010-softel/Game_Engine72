#include "Engine/Application.h"

int main() {
    Engine::ApplicationConfig config;
    Engine::Application app(config);
    app.run();

    return 0;
}
