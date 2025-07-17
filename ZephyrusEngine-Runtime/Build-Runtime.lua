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

   libdirs 
   {  
    "../Vendor/Dependencies/GL/SDL/lib/x86",
    "../Vendor/Dependencies/glew-2.2.0-win32/glew-2.2.0/bin/Release/Win32",
    "../Vendor/Dependencies/glew-2.2.0-win32/glew-2.2.0/lib/Release/Win32",
    "../Vendor/Dependencies/SDL2_image-2.8.2/lib/x86",
    "../Vendor/Dependencies/freetype-2.6.1/objs/vc2010/Win32",
   }

   links
   {
      "ZephyrusEngine-Core",
      "glu32", 
      "glew32",
      "opengl32",
      "SDL2",
      "SDL2main",
      "SDL2_image",
      "freetype261",
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
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