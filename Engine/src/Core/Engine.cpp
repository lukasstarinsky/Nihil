#include "Platform/Platform.hpp"
#include "Engine.hpp"

Engine::Engine(Application* application)
    : mApplication{application}
{
    NASSERT_MSG(application, "Application cannot be nullptr.");

    LOG_TRACE("Initializing...");
    if (!Platform::Initialize(mApplication->Config))
    {
        LOG_FATAL("Failed to initialize platform. Shutting down...");
        mApplication->State.IsRunning = false;
        return;
    }
}

Engine::~Engine()
{
    LOG_TRACE("Shutting down...");
    Platform::Shutdown();
}

void Engine::Run() const
{
    while (true);
}