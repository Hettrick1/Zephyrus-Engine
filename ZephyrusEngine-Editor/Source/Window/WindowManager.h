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
        bool mIsOpen = true;
        bool isMaximized = false;
        ImVec2 savedPos, savedSize;
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
            mIsOpen = true;
        }
        
        void DrawWindows();
        void Unload();
    };
}
