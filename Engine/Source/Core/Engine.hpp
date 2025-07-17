#pragma once

#include "Application.hpp"
#include "Event.hpp"

class NIHIL_API Engine
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