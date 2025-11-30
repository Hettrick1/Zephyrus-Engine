#pragma once

// --- Standard C++ ---
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <thread>
#include <functional>
#include <chrono>

// --- Math / Utils ---
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4D.h"


// --- GLFW ---
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// --- OpenGL loader (GLEW ou GLAD) ---
#include <glew.h>

// --- Imgui ---
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"