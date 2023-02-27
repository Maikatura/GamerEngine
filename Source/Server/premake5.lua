project "Server"
location "."
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
staticruntime "Off"
debugdir "%{wks.location}/bin"
configurations { Debug, Release}
configmap {
    ["Debug"] = "Debug",
    ["Release"] = "Release",
    ["Distribution"] = "Release"
}
targetdir ("%{wks.location}/bin/")
objdir ("%{wks.location}/Temp/Intermediate/%{prj.name}")
targetname("%{prj.name}_%{cfg.buildcfg}")

linkoptions
{
    "-ignore:4006",
    "-ignore:4099",
    "-ignore:4324",
}

libdirs 
{ 
	"%{wks.location}/lib"
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

includedirs{
    "."
}

externalincludedirs {
	"%{IncludeDirs.TurNet}",
	"%{IncludeDirs.Game}",
	"%{IncludeDirs.RapidJson}",
	"%{IncludeDirs.Components}",
	"%{IncludeDirs.CommonUtilities}"
}

externalwarnings "Off"

links
{
	"TurNet",
	"CommonUtilities",
	"Game",
	"ws2_32.lib"
}

files 
{
	"**.h",
	"**.cpp",
	"**.hpp"
}

flags
{
    "MultiProcessorCompile",
    "FatalCompileWarnings"
}

filter "system:windows"
    systemversion "latest"
    
filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "On"
	
    
filter {}
