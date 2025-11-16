#pragma once

#include <string>

namespace Zephyrus::Editor::Window
{
    class EditorWindow
    {
    protected:
        bool mIsOpen = true;
        bool mFocusNextFrame = false;
    public:
        virtual ~EditorWindow() = default;
        
        virtual std::string GetId() const = 0;
        
        virtual std::string GetTitle() const = 0;
        
        virtual void Draw() = 0;

        void Open()
        {
            mIsOpen = true;
            mFocusNextFrame = true;
        }
        bool IsOpen() const { return mIsOpen; }
        bool NeedFocusNextFrame() const { return mFocusNextFrame; }
        void Close()
        {
            mIsOpen = false;
            mFocusNextFrame = true;
        }
    };
}