#include "Platform/Platform.hpp"
#include "Renderer/Renderer.hpp"
#include "Engine.hpp"

Engine::Engine(Application* application)
    : mApplication{application}
{
    NASSERT_MSG(mApplication, "Application cannot be nullptr.");

    LOG_TRACE("Initializing...");
    if (!Platform::Initialize(mApplication->Config))
    {
        LOG_FATAL("Failed to initialize platform. Shutting down...");
        mApplication->State.IsRunning = false;
        return;
    }
    if (!Renderer::Initialize(RendererAPI::Vulkan))
    {
        LOG_FATAL("Failed to initialize Vulkan renderer. Shutting down...");
        mApplication->State.IsRunning = false;
        return;
    }

    SET_EVENT_LISTENER_THIS(EventCategory::Application, OnAppEvent);
}

Engine::~Engine()
{
    LOG_TRACE("Shutting down...");
    Renderer::Shutdown();
    Platform::Shutdown();
}

void Engine::Run() const
{
    while (mApplication->State.IsRunning)
    {
        if (!mApplication->State.IsSuspended)
        {
            mApplication->OnUpdate();
            mApplication->OnRender();
        }
        Platform::PollEvents();
    }
}

bool Engine::OnAppEvent(Event e)
{
    auto appEvent { static_cast<const ApplicationEvent*>(e) };

    if (appEvent->Type == ApplicationEventType::Quit)
    {
        mApplication->State.IsRunning = false;
        return true;
    }
    else if (appEvent->Type == ApplicationEventType::Resize)
    {
        mApplication->Config.WindowWidth = appEvent->Width;
        mApplication->Config.WindowHeight = appEvent->Height;

        if (appEvent->Width == 0 || appEvent->Height == 0)
        {
            mApplication->State.IsSuspended = true;
        }
        else
        {
            mApplication->State.IsSuspended = false;
        }
    }

    return false;
}