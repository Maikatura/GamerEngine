project "OpenFBX"
location "."
kind "StaticLib"
language "C++"
cppdialect "C++20"
staticruntime "Off"

targetdir ("%{wks.location}/Temp/lib/")
objdir ("%{wks.location}/Temp/Intermediate/%{cfg.buildcfg}/%{prj.name}")
targetname("%{cfg.buildcfg}/%{prj.name}")

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
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"_CRT_SECURE_NO_WARNINGS",
	"_HAS_EXCEPTIONS=0"
}

flags
{
    "MultiProcessorCompile"
}
includedirs
{
    "./src",
	"./include"
}

files {
	"**.h",
	"**.cpp",
	"**.hpp",
    "**.inl",
	"**.c"
}

externalincludedirs {
    
}

externalwarnings "Off"

defines{
    
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
