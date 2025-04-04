#include "Platform/Platform.hpp"
#include "Renderer/Renderer.hpp"
#include "Engine.hpp"

Engine::Engine(Application* application)
    : mApplication{application}
{
    ASSERT_MSG(mApplication, "Application cannot be nullptr.");

    Platform::Initialize(mApplication->Config);
    Renderer::Initialize(mApplication->Config);
    mApplication->OnInitialize();

    ADD_EVENT_LISTENER_THIS(Event::ApplicationQuit | Event::ApplicationResize, OnAppEvent);
}

Engine::~Engine()
{
    mApplication->OnShutdown();
    Renderer::Shutdown();
    Platform::Shutdown();
}

void Engine::Run() const
{
    while (mApplication->State.IsRunning)
    {
        if (!mApplication->State.IsSuspended)
        {
            Renderer::BeginFrame(0.4f, 0.4f, 0.4f, 1.0f);

            mApplication->OnUpdate();
            mApplication->OnRender();

            Renderer::EndFrame();
        }
        Platform::PollEvents();
    }
}

auto Engine::OnAppEvent(const Event& e) -> bool
{
    if (e.Type == Event::ApplicationQuit)
    {
        mApplication->State.IsRunning = false;
        return true;
    }
    else if (e.Type == Event::ApplicationResize)
    {
        const auto& appEvent = e.ApplicationEvent;

        mApplication->Config.WindowWidth = appEvent.Width;
        mApplication->Config.WindowHeight = appEvent.Height;

        if (appEvent.Width == 0 || appEvent.Height == 0)
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