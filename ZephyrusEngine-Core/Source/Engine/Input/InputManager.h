#pragma once
#include "InputAction.h"
#include "InputActionBool.h"
#include "InputActionAxis1D.h"
#include "InputActionAxis2D.h"
#include <unordered_map>

class ISceneContext;

// Manages input bindings and dispatches input events to registered listeners.
namespace Zephyrus::Inputs {
    class InputManager
    {
    private:
        GLFWwindow* mWindow;

        std::unordered_map<std::string, std::unique_ptr<InputAction>> mActions;

        Vector2D mMousePos;
        Vector2D mMouseDelta;

        bool mMouseWasActive{ false };
    public:
        InputManager(GLFWwindow* window);
        ~InputManager();

        void NewFrame();        // resets axis values
        void UpdateGamepad();   // poll gamepad

        void SetCursorVisible(bool visible)
        {
            if (visible)
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }

        void SetCursorRelative(bool relative)
        {
            if (relative)
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            else
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        
        InputActionBool& CreateBool(const std::string& name);
        InputActionAxis1D& CreateAxis1D(const std::string& name);
        InputActionAxis2D& CreateAxis2D(const std::string& name);

        InputAction* GetAction(const std::string& name);

        void OnKeyEvent(int key, int action);
        void OnMouseButtonEvent(int button, int action);
        void OnMouseMove(double xpos, double ypos);
        void OnScroll(double xoffset, double yoffset);
};

}