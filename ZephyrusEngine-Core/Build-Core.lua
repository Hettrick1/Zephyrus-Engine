project "ZephyrusEngine-Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",

       "../Imports",

      "../Vendor/Dependencies/glew-2.2.0-win32/glew-2.2.0/include/GL",
      "../Vendor/Dependencies/GL/SDL/include",
      "../Vendor/Dependencies/SDL2_image-2.8.2/include",
      "../Vendor/Dependencies/freetype-2.6.1/include",
      "../Vendor/Dependencies/freetype-2.6.1/include/freetype",

      "Source/Engine",
      "Source/Maths",
      "Source/Scenes",
      "Source/Engine/ActorComponents",
      "Source/Engine/DebugClasses",
      "Source/Engine/Editor",
      "Source/Engine/Graphics",
      "Source/Engine/ActorComponents/Colliders",
      "Source/Engine/ActorComponents/Managers",

      "Source/Engine/Inputs",
      "Source/Engine/Graphics/OpenGl",
      "Source/Engine/Graphics/SDL",
      "Source/Engine/ActorComponents/GraphicComponent",
      "Source/Engine/ActorComponents/PhysicComponent",
      "Source/Engine/ActorComponents/Actor",
      "Source/Engine/CoreComponents",
      "Source/Engine/Graphics/Hud",
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