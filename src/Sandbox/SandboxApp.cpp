#include "Engine/Application.h"

int main()
{
    Engine::ApplicationConfig config;
    config.name = "Sandbox Application";
    //config.maxFrames = 600;

    Engine::Application app(config);
    app.run();
}
