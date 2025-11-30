#include "pch.h"
#include "InputManager.h"

namespace Zephyrus::Inputs
{
   static void KeyCallback(GLFWwindow* win, int key, int sc, int action, int mods)
    {
        auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        if (input) input->OnKeyEvent(key, action);
    }

    static void MouseButtonCallback(GLFWwindow* win, int button, int action, int mods)
    {
        auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        if (input) input->OnMouseButtonEvent(button, action);
    }

    static void CursorPosCallback(GLFWwindow* win, double x, double y)
    {
        auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        if (input) input->OnMouseMove(x, y);
    }

    static void ScrollCallback(GLFWwindow* win, double xoff, double yoff)
    {
        auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        if (input) input->OnScroll(xoff, yoff);
    }

    // ---- Constructor ---- //

    InputManager::InputManager(GLFWwindow* window)
        : mWindow(window)
    {
        glfwSetWindowUserPointer(window, this);

        glfwSetKeyCallback(window, KeyCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetCursorPosCallback(window, CursorPosCallback);
        glfwSetScrollCallback(window, ScrollCallback);
    }

    InputManager::~InputManager()
   {
       mActions.clear();
   }


    // ---- Create actions ---- //

    InputActionBool& InputManager::CreateBool(const std::string& name)
    {
        mActions[name] = std::make_unique<InputActionBool>(name);
        return *static_cast<InputActionBool*>(mActions[name].get());
    }

    InputActionAxis1D& InputManager::CreateAxis1D(const std::string& name)
    {
        mActions[name] = std::make_unique<InputActionAxis1D>(name);
        return *static_cast<InputActionAxis1D*>(mActions[name].get());
    }

    InputActionAxis2D& InputManager::CreateAxis2D(const std::string& name)
    {
        mActions[name] = std::make_unique<InputActionAxis2D>(name);
        return *static_cast<InputActionAxis2D*>(mActions[name].get());
    }

    InputAction* InputManager::GetAction(const std::string& name)
    {
        auto it = mActions.find(name);
        return (it != mActions.end()) ? it->second.get() : nullptr;
    }


    // ---- Frame update ---- //

    void InputManager::NewFrame()
    {
        // reset all axis
        for (auto& [name, action] : mActions)
            {
            if (action->GetType() == ActionType::Axis1D)
            {
                static_cast<InputActionAxis1D*>(action.get())->Reset();
            }

            if (action->GetType() == ActionType::Axis2D)
            {
                static_cast<InputActionAxis2D*>(action.get())->Reset();
            }
        }
    }

    void InputManager::UpdateGamepad()
    {
        // Optional: implement later
    }


    // ---- Callbacks ---- //

    void InputManager::OnKeyEvent(int key, int action)
    {
        for (auto& [name, act] : mActions)
            {

            if (act->IsBoundToKey(key))
                {
                switch (act->GetType())
                {
                    case ActionType::Boolean:
                    {
                        auto* b = static_cast<InputActionBool*>(act.get());
                        if (action == GLFW_PRESS) { b->IsDown = true; b->TriggerStarted(); }
                        if (action == GLFW_REPEAT) { b->TriggerTriggered(); }
                        if (action == GLFW_RELEASE){ b->IsDown = false; b->TriggerReleased(); }
                    }
                    break;

                    case ActionType::Axis1D:
                    {
                        auto* a = static_cast<InputActionAxis1D*>(act.get());
                        if (a->keyValues.contains(key))
                            {
                            if (action == GLFW_PRESS) {
                                a->IsDown = true;
                                a->TriggerStarted();
                                a->TriggerTriggered(a->keyValues[key]);
                            }
                            if (action == GLFW_REPEAT) { a->TriggerTriggered(a->keyValues[key]); }
                            if (action == GLFW_RELEASE){ a->IsDown = false; a->TriggerReleased(); }
                        }
                    }
                    break;

                    case ActionType::Axis2D:
                    {
                        auto* a = static_cast<InputActionAxis2D*>(act.get());
                        if (a->keyValues.contains(key)) {
                            if (action == GLFW_PRESS) {
                                a->IsDown = true;
                                a->TriggerStarted();
                                a->TriggerTriggered(a->keyValues[key]);
                            }
                            if (action == GLFW_REPEAT) { a->TriggerTriggered(a->keyValues[key]); }
                            if (action == GLFW_RELEASE){ a->IsDown = false; a->TriggerReleased(); }
                        }
                    }
                    break;
                }
            }
        }
    }

    void InputManager::OnMouseButtonEvent(int button, int action)
    {
       for (auto& [name, act] : mActions)
       {
           if (!act->IsBoundToMouse(button))
               continue;

           switch (act->GetType())
           {
           case ActionType::Boolean:
               {
                   auto* b = static_cast<InputActionBool*>(act.get());
                   if (action == GLFW_PRESS)    { b->IsDown = true;  b->TriggerStarted(); }
                   if (action == GLFW_REPEAT)   { b->TriggerTriggered(); }
                   if (action == GLFW_RELEASE)  { b->IsDown = false; b->TriggerReleased(); }
               }
               break;

           case ActionType::Axis1D:
               {
                   auto* a = static_cast<InputActionAxis1D*>(act.get());
                   if (a->mouseValues.contains(button)) {
                       float v = a->mouseValues[button];

                       if (action == GLFW_PRESS)    { a->IsDown = true;  a->TriggerStarted(); }
                       if (action == GLFW_REPEAT)   { a->TriggerTriggered(v); }
                       if (action == GLFW_RELEASE)  { a->IsDown = false; a->TriggerReleased(); }
                   }
               }
               break;

           case ActionType::Axis2D:
               {
                   auto* a = static_cast<InputActionAxis2D*>(act.get());
                   if (a->mouseValues.contains(button)) {
                       Vector2D v = a->mouseValues[button];

                       if (action == GLFW_PRESS)    { a->IsDown = true;  a->TriggerStarted(); }
                       if (action == GLFW_REPEAT)   { a->TriggerTriggered(v); }
                       if (action == GLFW_RELEASE)  { a->IsDown = false; a->TriggerReleased(); }
                   }
               }
               break;
           }
       }
    }

    void InputManager::OnMouseMove(double xpos, double ypos)
   {
       Vector2D delta = { (float)xpos - mLastCenter.x, (float)ypos - mLastCenter.y };
    
       // Update mouse pos
       mMousePos += delta;

       // Recenter the cursor
       glfwSetCursorPos(mWindow, mLastCenter.x, mLastCenter.y);

       // Send delta to all mouse axes
       for (auto& [name, act] : mActions)
       {
           if (act->GetType() == ActionType::Axis2D && act->IsMouseAxis)
           {
               auto* axis = static_cast<InputActionAxis2D*>(act.get());
            
               if (!mMouseWasActive && (delta.x != 0 || delta.y != 0))
               {
                   mMouseWasActive = true;
                   axis->TriggerStarted();
               }

               if (delta.x != 0 || delta.y != 0)
                   axis->TriggerTriggered(delta);
               else if (mMouseWasActive)
               {
                   mMouseWasActive = false;
                   axis->TriggerReleased();
               }
           }
       }
    }

    void InputManager::OnScroll(double xoffset, double yoffset)
    {
        // You can emit a scroll action if needed
    }
}