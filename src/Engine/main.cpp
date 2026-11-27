#include "Engine/Application.h"

int main() {
    Engine::ApplicationConfig config;
    config.trainQLearning = false;
    config.miniGameMode = false;
    config.habitatMode = false;
    config.battleArenaMode = false;
    config.ruleBasedMode = true;
    Engine::Application app(config);
    app.run();

    return 0;
}
