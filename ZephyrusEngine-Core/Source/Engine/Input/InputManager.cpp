#include "pch.h"
#include "InputManager.h"

namespace Zephyrus::Inputs
{
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

    void InputManager::UpdateKeysAndButtons()
    {
        for (auto& [name, actPtr] : mActions)
        {
            auto* act = actPtr.get();
            switch (act->GetType())
            {
                case ActionType::Boolean:
                {
                    auto* b = static_cast<InputActionBool*>(act);
                    bool anyPressed = false;
                    for (int key : b->GetKeys())
                    {
                        if (glfwGetKey(mWindow, key) == GLFW_PRESS)
                        {
                            anyPressed = true;
                            break;
                        }
                    }

                    if (anyPressed)
                    {
                        if (!b->IsDown) { b->IsDown = true; b->TriggerStarted(); }
                        b->TriggerTriggered();
                    }
                    else
                    {
                        if (b->IsDown) { b->IsDown = false; b->TriggerReleased(); }
                    }
                }
                break;

                case ActionType::Axis1D:
                {
                    auto* a = static_cast<InputActionAxis1D*>(act);
                    float total = 0.0f;
                    for (auto& [key, val] : a->GetKeyValues())
                    {
                        if (glfwGetKey(mWindow, key) == GLFW_PRESS)
                            total += val;
                    }

                    if (total != 0.0f)
                    {
                        if (!a->IsDown) { a->IsDown = true; a->TriggerStarted(); }
                        a->TriggerTriggered(total);
                    }
                    else
                    {
                        if (a->IsDown) { a->IsDown = false; a->TriggerReleased(); }
                    }
                }
                break;

                case ActionType::Axis2D:
                {
                    auto* a = static_cast<InputActionAxis2D*>(act);
                    Vector2D total { 0.0f, 0.0f };
                    for (auto& [key, value] : a->GetKeyValues())
                    {
                        if (glfwGetKey(mWindow, key) == GLFW_PRESS)
                            total += value;
                    }

                    if (total.x != 0.0f || total.y != 0.0f)
                    {
                        if (!a->IsDown) { a->IsDown = true; a->TriggerStarted(); }
                        a->TriggerTriggered(total);
                    }
                    else
                    {
                        if (a->IsDown) { a->IsDown = false; a->TriggerReleased(); }
                    }
                }
                break;
            }
        }
    }

    void InputManager::UpdateGamepad()
    {
        // Optional: implement later
    }


    // ---- Callbacks ---- //

    void InputManager::OnMouseMove(double xpos, double ypos)
   {
       Vector2D delta = { (float)xpos - mMousePos.x, (float)ypos - mMousePos.y };
       
       // Update mouse pos
       mMousePos += delta;

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

    void InputManager::SetPriority()
    {
       glfwSetWindowUserPointer(mWindow, this);
    }
}
