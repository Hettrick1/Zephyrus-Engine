project "ZephyrusEngine-Runtime"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
      "Source/Childs",
      "Source/Game",

      "../Imports",

      "../Vendor/Dependencies/glew-2.2.0-win32/glew-2.2.0/include/GL",
      "../Vendor/Dependencies/GL/SDL/include",
      "../Vendor/Dependencies/SDL2_image-2.8.2/include",
      "../Vendor/Dependencies/freetype-2.6.1/include",
      "../Vendor/Dependencies/freetype-2.6.1/include/freetype",
      "../Vendor/Dependencies/rapidjson-1.1.0/include",

	  -- Include Core
	  "../ZephyrusEngine-Core/Source",

      "../ZephyrusEngine-Core/Source/Engine",
      "../ZephyrusEngine-Core/Source/Maths",
      "../ZephyrusEngine-Core/Source/Scenes",
      "../ZephyrusEngine-Core/Source/Engine/ActorComponents",
      "../ZephyrusEngine-Core/Source/Engine/DebugClasses",
      "../ZephyrusEngine-Core/Source/Engine/Editor",
      "../ZephyrusEngine-Core/Source/Engine/Graphics",
      "../ZephyrusEngine-Core/Source/Engine/ActorComponents/Colliders",
      "../ZephyrusEngine-Core/Source/Engine/ActorComponents/Managers",

      "../ZephyrusEngine-Core/Source/Engine/Inputs",
      "../ZephyrusEngine-Core/Source/Engine/Graphics/OpenGl",
      "../ZephyrusEngine-Core/Source/Engine/Graphics/SDL",
      "../ZephyrusEngine-Core/Source/Engine/ActorComponents/GraphicComponent",
      "../ZephyrusEngine-Core/Source/Engine/ActorComponents/PhysicComponent",
      "../ZephyrusEngine-Core/Source/Engine/ActorComponents/Actor",
      "../ZephyrusEngine-Core/Source/Engine/CoreComponents",
      "../ZephyrusEngine-Core/Source/Engine/Graphics/Hud",
   }

   links
   {
      "ZephyrusEngine-Core",
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   postbuildcommands
   {
    "{COPY} ../Vendor/Dependencies/GL/SDL/lib/x86/SDL2.dll %{cfg.targetdir}",
    "{COPY} ../Vendor/Dependencies/glew-2.2.0-win32/glew-2.2.0/bin/Release/Win32/glew32.dll %{cfg.targetdir}",
    "{COPY} ../Vendor/Dependencies/SDL2_image-2.8.2/lib/x86/SDL2_image.dll %{cfg.targetdir}",
   }

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