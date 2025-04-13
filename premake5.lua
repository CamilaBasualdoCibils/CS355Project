-- premake5.lua
workspace "Digipen"
   configurations { "Debug", "Release" }

project "Mergesort"

   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   files {"src/**.c","src/**.cpp"}
   --pchheader "pch.hpp"
   includedirs {"src","lib"}
   --links { }


   filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "_NDEBUG","_RELEASE" }
      optimize "On"
      symbols "On"


   --filter "action:gmake" IMPORTANT FOR COMPILING IN LINUX FOR WINDOWS
      --gccprefix "x86_64-w64-mingw32-" --compilation in linux for windows

