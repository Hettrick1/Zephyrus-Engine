#pragma once

#include "../EditorWindow.h"
#include "../../Panel/InspectorPanel/ComponentPropertyDrawer.h"
#include "Material/IMaterial.h"

namespace Zephyrus::Editor::Window
{
    class MaterialWindow : public EditorWindow
    {
    private:
        std::string mFilePath;
        std::string mTitle;
        ComponentPropertyDrawer* mComponentPropertyDrawer{ nullptr };
        Material::IMaterial* mMaterial{ nullptr };
        Material::IMaterial* mMaterialCopy{ nullptr };
    public:
        MaterialWindow(const std::string& filePath, const std::string& title);
        ~MaterialWindow() override;
        
        std::string GetId() const override { return mFilePath; }
        std::string GetTitle() const override { return "Material - " + mTitle; } // TODO : Modify the title name

        void Draw() override;
    }; 
}

