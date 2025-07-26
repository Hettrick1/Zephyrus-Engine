-- premake5.lua
workspace "Zephyrus Engine"
   architecture "x86"
   configurations { "Debug", "Release", "Dist" }
   startproject "ZephyrusEngine-Runtime"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

filter "system:windows"
   postbuildcommands
   {
      'rmdir /S /Q "%{cfg.targetdir}\\..\\Imports"',
      '{COPYDIR} ../Imports "%{cfg.targetdir}\\..\\Imports"'
   }
filter "system:linux"
   postbuildcommands
   {
      'rm -rf "%{cfg.targetdir}\\..\\Imports"',
      '{COPYDIR} ../Imports "%{cfg.targetdir}\\..\\Imports"'
   }

group "Core"
	include "ZephyrusEngine-Core/Build-Core.lua"
group ""

include "ZephyrusEngine-Editor/Build-Editor.lua"
include "ZephyrusEngine-Runtime/Build-Runtime.lua"
