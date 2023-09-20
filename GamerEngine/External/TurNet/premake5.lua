project "TurNet"
location "."
kind "StaticLib"
language "C++"
cppdialect "C++20"
staticruntime "Off"
debugdir "%{wks.location}/lib"
configurations { Debug, Release}
configmap {
    ["Debug"] = "Debug",
    ["Release"] = "Release",
    ["Distribution"] = "Release"
}

targetdir ("%{wks.location}/Temp/lib/")
objdir ("%{wks.location}/Temp/Intermediate/%{prj.name}")
targetname("%{prj.name}_%{cfg.buildcfg}")

linkoptions
{
    "-ignore:4006",
    "-ignore:4099",
    "-ignore:4324",
	"-ignore:4231",
	"-ignore:4251",
	"-ignore:4275",
	"-ignore:4355",
	"-ignore:4505",
	"-ignore:4244",
	"-ignore:4242"
}

disablewarnings
{
    "26812", --prefer enum class over enum
    "26451",	--arithmetic overflow
	"4244"
}

libdirs 
{ 
	"%{wks.location}/lib"
}

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
}

includedirs
{
    ".",
}

links 
{
	"ws2_32.lib"
}


externalwarnings "Off"

files 
{
	"**.h",
	"**.cpp",
	"**.hpp"
}

flags
{
    "MultiProcessorCompile"
}

filter "system:windows"
    systemversion "latest"
    
	
filter "configurations:Debug"
    defines {
		"_DEBUG"
	}
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "On"
	
filter {}

