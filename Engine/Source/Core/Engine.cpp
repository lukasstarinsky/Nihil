#include "Platform/Platform.hpp"
#include "Graphics/Renderer.hpp"
#include "Engine.hpp"

Engine::Engine(Application* application)
    : mApplication{application}
{
    ASSERT(mApplication);

    Platform::Initialize(mApplication->Config);
    Renderer::Initialize(mApplication->Config);
    mApplication->OnInitialize();

    EventDispatcher::AddListener<ApplicationEvent>(std::bind_front(&Engine::OnAppEvent, this));
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
        f64 time = Platform::GetTimeSeconds();
        f64 deltaTimeSeconds = time - mApplication->State.LastFrameTime;
        mApplication->State.LastFrameTime = time;

        if (!mApplication->State.IsSuspended)
        {
            Renderer::BeginFrame(0.1f, 0.1f, 0.1f, 1.0f);

            mApplication->OnUpdate(static_cast<f32>(deltaTimeSeconds));
            mApplication->OnRender();

            Renderer::EndFrame();
        }
        Platform::PollEvents();
    }
}

auto Engine::OnAppEvent(const ApplicationEvent& e) -> bool
{
    if (e.Type == EventType::ApplicationQuit)
    {
        mApplication->State.IsRunning = false;
        return true;
    }
    else if (e.Type == EventType::ApplicationResize)
    {
        mApplication->Config.WindowWidth = e.Width;
        mApplication->Config.WindowHeight = e.Height;

        if (e.Width == 0 || e.Height == 0)
        {
            mApplication->State.IsSuspended = true;
            return true;
        }
        else
        {
            mApplication->State.IsSuspended = false;
            mApplication->OnResize();
        }
    }

    return false;
}