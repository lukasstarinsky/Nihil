#pragma once

#include "Application.hpp"

class NIHIL_API Engine
{
public:
    explicit Engine(Application* application);
    ~Engine();

    void Run() const;
private:
    Application* mApplication {};
};