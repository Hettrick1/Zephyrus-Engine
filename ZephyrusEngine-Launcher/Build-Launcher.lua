project "ZephyrusEngine-Launcher"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    targetname ("Zephyrus")

    files { "Source/**.h", "Source/**.cpp", "Source/**.rc" }

    linkoptions { "/ENTRY:mainCRTStartup" }

    includedirs
    {
        "Source"
    }

    targetdir ("../Binaries/" .. OutputDir .. "")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

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