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
    EventDispatcher::Dispatch(ApplicationEvent{ .Width = application->Config.WindowWidth, .Height = application->Config.WindowHeight, .Type = EventType::ApplicationResize });
}

Engine::~Engine()
{
    mApplication->OnShutdown();
    Renderer::Shutdown();
    Platform::Shutdown();
}

void Engine::Run() const
{
    mApplication->State.LastFrameTime = std::chrono::steady_clock::now();

    while (mApplication->State.IsRunning)
    {
        auto time = std::chrono::steady_clock::now();
        auto deltaTime = time - mApplication->State.LastFrameTime;

        mApplication->State.LastFrameTime = time;
        auto deltaTimeSeconds = std::chrono::duration<f32>(deltaTime).count();

        if (!mApplication->State.IsSuspended)
        {
            Renderer::BeginFrame(0.1f, 0.1f, 0.1f, 1.0f);

            Renderer::Enable(RenderState::DepthTest);
            Renderer::Enable(RenderState::CullFace);
            mApplication->OnUpdate(deltaTimeSeconds);
            mApplication->OnRender();

            Renderer::Disable(RenderState::DepthTest);
            Renderer::Disable(RenderState::CullFace);
            mApplication->OnUIRender();

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
            mApplication->OnWindowResize();
        }
    }

    return false;
}