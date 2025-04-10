#pragma once

#include <Core/Defines.hpp>
#include <Core/Logger.hpp>
#include <Core/Engine.hpp>
#include <Core/Exception.hpp>
#include <Core/Application.hpp>
#include <Graphics/Renderer.hpp>
#include <Platform/Platform.hpp>

#define CREATE_APPLICATION(Application)                 \
try {                                                   \
    Application application;                            \
    Engine engine(&application);                        \
    engine.Run();                                       \
    return EXIT_SUCCESS;                                \
} catch (const NihilException& e) {                     \
    Logger::Fatal("{}", e);                             \
    return EXIT_FAILURE;                                \
} catch (...) {                                         \
    Logger::Fatal("Unknown exception occured.");        \
    return EXIT_FAILURE;                                \
} while(0)