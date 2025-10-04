-- premake5.lua
workspace "Zephyrus Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ZephyrusEngine-Runtime"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

filter "system:windows"
   postbuildcommands
   {
      'rmdir /S /Q "%{cfg.targetdir}\\..\\Content"',
      '{COPYDIR} ../Content "%{cfg.targetdir}\\..\\Content"',
      'rmdir /S /Q "%{cfg.targetdir}\\..\\Config"',
      '{COPYDIR} ../Config "%{cfg.targetdir}\\..\\Config"'
   }
filter "system:linux"
   postbuildcommands
   {
      'rm -rf "%{cfg.targetdir}\\..\\Content"',
      '{COPYDIR} ../Content "%{cfg.targetdir}\\..\\Content"'
   }

group "Core"
	include "ZephyrusEngine-Core/Build-Core.lua"
group ""

include "ZephyrusEngine-Mathematics/Build-Mathematics.lua"
include "ZephyrusEngine-Editor/Build-Editor.lua"
include "ZephyrusEngine-Runtime/Build-Runtime.lua"
