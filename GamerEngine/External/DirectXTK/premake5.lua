project "DirectXTK"
location "."
kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "Off"

targetdir ("%{wks.location}/Temp/lib/")
objdir ("%{wks.location}/Temp/Intermediate/%{prj.name}_%{cfg.buildcfg}")
targetname("%{prj.name}_%{cfg.buildcfg}")

pchheader "src/pch.h"
pchsource "src/pch.cpp"

linkoptions
{
    "-ignore:4006",
    "-ignore:4099",
}

disablewarnings
{
    "26812", --prefer enum class over enum
    "26451" --arithmetic overflow
}

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
}

flags
{
    "MultiProcessorCompile"
}

files {
	"./inc/*.h",
	"./inc/*.cpp",
	"./src/*.h",
	"./src/*.cpp",
	"./inc/*.hpp",
	"./src/*.hpp",
   	 "**.inl",
	"./src/Shaders/Compiled/*.inc"
}

includedirs{
    	"./src",
    	"./inc",
	"./src/Shaders/Compiled"
}

externalincludedirs {
    
}

externalwarnings "Off"

defines{
    "FBXSDK_SHARED"
}

libdirs {
}

filter "system:windows"
    systemversion "latest"


filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "On"
    links {
    }

filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "On"
    links {
    }

filter "configurations:Distribution"
    defines "_DISTRIBUTION"
    runtime "Release"
    optimize "On"
	symbols "Off"
    links {
    }

filter{}
