project "Audio"
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
objdir ("%{wks.location}/Temp/Intermediate/%{cfg.buildcfg}/%{prj.name}")
targetname("%{cfg.buildcfg}/%{prj.name}")

linkoptions
{
    "-ignore:4006",
    "-ignore:4099",
    "-ignore:4324",
}

disablewarnings
{
    "26812", --prefer enum class over enum
    "26451" --arithmetic overflow
}

libdirs 
{ 
	"%{wks.location}/lib"
}

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
}

includedirs{
    "./src",

}

externalincludedirs {
	
}

externalwarnings "Off"

links
{
}

files 
{
	"**.h",
	"**.cpp",
	"**.hpp",
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
