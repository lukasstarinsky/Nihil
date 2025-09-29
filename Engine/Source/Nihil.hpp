#pragma once

#include <Common/Defines.hpp>
#include <Common/Logger.hpp>
#include <Common/Exception.hpp>
#include <Common/Application.hpp>
#include <Core/Engine.hpp>
#include <AssetPipeline//RawAssetManager.hpp>
#include <AssetPipeline//PackedAssetManager.hpp>
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
} catch (const std::exception& e) {                     \
    Logger::Fatal("{}", e.what());                      \
    return EXIT_FAILURE;                                \
} catch (...) {                                         \
    Logger::Fatal("Unknown exception occured.");        \
    return EXIT_FAILURE;                                \
} while(0)