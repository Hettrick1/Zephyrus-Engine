#pragma once

#include "../EditorWindow.h"

namespace Zephyrus::Editor::Window
{
    class MaterialWindow : public EditorWindow
    {
    private:
        std::string mFilePath;
        std::string mTitle;
    public:
        MaterialWindow(const std::string& filePath, const std::string& title);

        std::string GetId() const override { return mFilePath; }
        std::string GetTitle() const override { return "Material - " + mTitle; } // TODO : Modify the title name

        void Draw() override;
    }; 
}

