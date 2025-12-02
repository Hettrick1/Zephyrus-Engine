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
        Vector2D mLastCenter;

        bool isMouseRelative{false};

        bool mMouseWasActive{ false };
    public:
        InputManager(GLFWwindow* window);
        ~InputManager();

        void NewFrame(); // resets axis values
        void UpdateKeysAndButtons(); // poll keyboard and mouse for trigger events
        void UpdateGamepad();   // poll gamepad

        void SetCursorVisible(bool visible) const
        {
            if (visible)
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }

        void SetCursorRelative(bool relative)
        {
            if (relative)
            {
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                double x, y;
                glfwGetCursorPos(mWindow, &x, &y);
                mMousePos.x = static_cast<float>(x);
                mMousePos.y = static_cast<float>(y);
                isMouseRelative = true;
            }
            else
            {
                glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                isMouseRelative = false;
            }
        }
        
        InputActionBool& CreateBool(const std::string& name);
        InputActionAxis1D& CreateAxis1D(const std::string& name);
        InputActionAxis2D& CreateAxis2D(const std::string& name);

        InputAction* GetAction(const std::string& name);

        void OnMouseMove();
        void OnScroll(double xoffset, double yoffset);

        void SetPriority();
        bool HasPriority() const
        {
            if (glfwGetWindowUserPointer(mWindow) == this)
            {
                return true;
            }
            return false;
        }
};

}