project "ZephyrusEngine-Editor"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    linkoptions { "/ENTRY:mainCRTStartup" }

    files { 
        "Source/**.h", 
        "Source/**.cpp", 
        "Source/**.rc",

        -- ImGui core
        "../Vendor/imgui-1.92.2b/imgui.cpp",
        "../Vendor/imgui-1.92.2b/imgui_draw.cpp",
        "../Vendor/imgui-1.92.2b/imgui_widgets.cpp",
        "../Vendor/imgui-1.92.2b/imgui_tables.cpp",
        "../Vendor/imgui-1.92.2b/imgui_demo.cpp", -- optional

        -- ImGui backends
        "../Vendor/imgui-1.92.2b/backends/imgui_impl_glfw.cpp",
        "../Vendor/imgui-1.92.2b/backends/imgui_impl_opengl3.cpp",

        -- ImGuizmo
        "../Vendor/ImGuizmo-1.83/ImGuizmo.cpp",
        "../Vendor/ImGuizmo-1.83/ImSequencer.cpp",
        }

    includedirs
    {
        "Source",

        "../Content",

        "../Vendor/glew-2.2.0-win32/glew-2.2.0/include/GL",
        "../Vendor/glfw-3.4/include",
        "../Vendor/freetype-2.6.1/include",
        "../Vendor/freetype-2.6.1/include/freetype",
        "../Vendor/rapidjson-1.1.0/include",
        "../Vendor/TinyObjLoader",
        "../Vendor/stb-image",
        "../Vendor/imgui-1.92.2b",
        "../Vendor/imgui-1.92.2b/backends",
        "../Vendor/bullet3-3.25/src",
        "../Vendor/ImGuizmo-1.83",

        "../ZephyrusEngine-Mathematics/Source",

        -- Include Core
        "../ZephyrusEngine-Core/Source",

        "../ZephyrusEngine-Core/Source/Engine",
        "../ZephyrusEngine-Core/Source/Scenes",
        "../ZephyrusEngine-Core/Source/Utils",

        "../ZephyrusEngine-Core/Source/Engine/Actor",
        "../ZephyrusEngine-Core/Source/Engine/Application",
        "../ZephyrusEngine-Core/Source/Engine/Component",
        "../ZephyrusEngine-Core/Source/Engine/Debug",
        "../ZephyrusEngine-Core/Source/Engine/Factory",
        "../ZephyrusEngine-Core/Source/Engine/Input",
        "../ZephyrusEngine-Core/Source/Engine/Physics",
        "../ZephyrusEngine-Core/Source/Engine/Rendering",

        "../ZephyrusEngine-Core/Source/Engine/Component",
        "../ZephyrusEngine-Core/Source/Engine/Component/ColliderComponent",
        "../ZephyrusEngine-Core/Source/Engine/Component/GraphicComponent",
        "../ZephyrusEngine-Core/Source/Engine/Component/PhysicComponent",

        "../ZephyrusEngine-Core/Source/Engine/Rendering/Assets",
        "../ZephyrusEngine-Core/Source/Engine/Rendering/Assets/OpenGl",
        "../ZephyrusEngine-Core/Source/Engine/Rendering/Assets/Shader",
        "../ZephyrusEngine-Core/Source/Engine/Rendering/OpenGl",
        "../ZephyrusEngine-Core/Source/Engine/Rendering/SDL",

        "../ZephyrusEngine-Core/Source/Engine/UI",
        "../ZephyrusEngine-Core/Source/Engine/UI/HudElement",

        "../ZephyrusEngine-Core/Source/Utils/Math"
    }

    links
    {
        "ZephyrusEngine-Core",
        "ZephyrusEngine-Mathematics"
    }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    postbuildcommands
    {
        "{COPY} ../Vendor/glew-2.2.0-win32/glew-2.2.0/bin/Release/x64/glew32.dll %{cfg.targetdir}"
    }

    filter "configurations:Debug"
    postbuildcommands
    {
        "{COPY} ../Vendor/glfw-3.4/libs/x64/Debug/glfw3.dll %{cfg.targetdir}"
    }

    filter "configurations:Release or configurations:Dist"
    postbuildcommands
    {
        "{COPY} ../Vendor/glfw-3.4/libs/x64/Release/glfw3.dll %{cfg.targetdir}"
    }

    filter {} -- reset filter

    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS" }

    filter "configurations:Debug"
        defines { "_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "Off"