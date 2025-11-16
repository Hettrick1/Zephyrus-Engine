#pragma once

#include "EditorWindow.h"
#include <memory>
#include <unordered_map>
#include <imgui_internal.h>

namespace Zephyrus::Editor::Window
{
    class WindowManager
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<EditorWindow>> mWindows;
        bool mIsOpen = false;
    public:
        template<typename T, typename... Args>
        void OpenWindow(const std::string& id, Args&&... args)
        {
            if (mWindows.find(id) != mWindows.end())
            {
                mWindows[id]->Open();
                mIsOpen = true;
                return;
            }
            
            std::shared_ptr<EditorWindow> window = std::make_shared<T>(id, std::forward<Args>(args)...);
            mWindows[id] = window;
        }
        
        void DrawWindows();
        void Unload();
    };
}
