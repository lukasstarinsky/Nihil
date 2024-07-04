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
    bool OnAppEvent(Event e);
private:
    Application* mApplication {};
};