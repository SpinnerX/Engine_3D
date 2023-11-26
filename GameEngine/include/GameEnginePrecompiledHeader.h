#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdarg>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <thread>

// OpenGL includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GameEngine/Events/ApplicationEvent.h>

#include <GameEngine/EngineLogger.h>

#include <fmt/ostream.h>
#if RENDER_PLATFORM_WINDOWS
    #include <Windows.h> // include Windows.h if we are in Windows platform
#endif