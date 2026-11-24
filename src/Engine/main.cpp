#include "Engine/Application.h"

int main() {
    Engine::ApplicationConfig config;
    config.trainQLearning = false;
    Engine::Application app(config);
    app.run();

    return 0;
}
