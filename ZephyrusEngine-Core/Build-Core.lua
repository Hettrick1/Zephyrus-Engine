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

      "../Content",

      "../Vendor/glew-2.2.0-win32/glew-2.2.0/include/GL",
      "../Vendor/GL/SDL/include",
      "../Vendor/SDL2_image-2.8.2/include",
      "../Vendor/freetype-2.6.1/include",
      "../Vendor/freetype-2.6.1/include/freetype",
      "../Vendor/rapidjson-1.1.0/include",
      "../Vendor/TinyObjLoader",
      "../Vendor/reactphysics3d-v0.10.2-0/include",

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
    "../Vendor/GL/SDL/lib/x86",
    "../Vendor/glew-2.2.0-win32/glew-2.2.0/bin/Release/Win32",
    "../Vendor/glew-2.2.0-win32/glew-2.2.0/lib/Release/Win32",
    "../Vendor/SDL2_image-2.8.2/lib/x86",
    "../Vendor/freetype-2.6.1/objs/vc2010/Win32",
    "../Vendor/reactphysics3d-v0.10.2-0/build/Debug",
   }

   links
   {
      "glu32", 
      "glew32",
      "opengl32",
      "SDL2",
      "SDL2_image",
      "SDL2main",
      "freetype261",
      "reactphysics3d"
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