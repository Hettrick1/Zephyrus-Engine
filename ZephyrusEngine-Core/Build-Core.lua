project "ZephyrusEngine-Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "Source/**.h", "Source/**.cpp" }

    pchheader "pch.h"
    pchsource "Source/pch.cpp"

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
        "../Vendor/stduuid-1.2.3/include",
        "../Vendor/bullet3-3.25/src",

        -- ENGINE INCLUDES --

        "../ZephyrusEngine-Mathematics/Source",

        "Source/Engine",
        "Source/Scenes",
        "Source/Utils",

        "Source/Engine/Actor",
        "Source/Engine/Application",
        "Source/Engine/Component",
        "Source/Engine/Debug",
        "Source/Engine/Factory",
        "Source/Engine/Input",
        "Source/Engine/Physics",
        "Source/Engine/Rendering",

        "Source/Engine/Component",
        "Source/Engine/Component/ColliderComponent",
        "Source/Engine/Component/GraphicComponent",
        "Source/Engine/Component/PhysicComponent",

        "Source/Engine/Rendering/Assets",
        "Source/Engine/Rendering/Assets/OpenGl",
        "Source/Engine/Rendering/Assets/Shader",
        "Source/Engine/Rendering/OpenGl",
        "Source/Engine/Rendering/SDL",

        "Source/Engine/UI",
        "Source/Engine/UI/HudElement",

        "Source/Utils/Math"
    }

    libdirs 
    {  
        "../Vendor/glew-2.2.0-win32/glew-2.2.0/bin/Release/x64",
        "../Vendor/glew-2.2.0-win32/glew-2.2.0/lib/Release/x64", -- THAT IS NOT THE SAME FILE
        "../Vendor/freetype-2.6.1/objs/vc2010/x64",
        "../Vendor/bullet3-3.25/lib/Release",
    }

    links
    {
        "ZephyrusEngine-Mathematics",
        "glu32", 
        "glew32",
        "opengl32",
        "freetype261"
    }

        -- Bullet libs selon la configuration
        filter "configurations:Debug"
        libdirs 
        { 
            "../Vendor/bullet3-3.25/lib/Debug",
            "../Vendor/glfw-3.4/libs/x64/Debug", 
        }
        links
        {
            "BulletCollision_Debug",
            "BulletDynamics_Debug",
            "Bullet3Common_Debug",
            "Bullet3Geometry_Debug",
            "LinearMath_Debug",

            "glfw3dll"
        }

        filter "configurations:Release or configurations:Dist"
        libdirs 
        { 
            "../Vendor/bullet3-3.25/lib/Release",
            "../Vendor/glfw-3.4/libs/x64/Release", 
        }
        links
        {
            "BulletCollision",
            "BulletDynamics",
            "Bullet3Common",
            "Bullet3Geometry",
            "LinearMath",

            "glfw3dll"
        }

        filter {} -- reset filter

    filter {}

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { }

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