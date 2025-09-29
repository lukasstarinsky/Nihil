#pragma once

#include "Common/Application.hpp"
#include "Common/Event.hpp"

class Engine
{
public:
    explicit Engine(Application* application);
    ~Engine();

    void Run() const;
private:
    auto OnAppEvent(const ApplicationEvent& e) -> bool;
private:
    Application* mApplication {};
};