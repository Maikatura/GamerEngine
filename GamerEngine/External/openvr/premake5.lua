project "OpenVR"
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
    "26451", --arithmetic overflow
	"2220"
}

libdirs 
{ 
	"%{wks.location}/lib"
}

defines
{
    	"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"NOMINMAX",
	"VR_API_PUBLIC",
	"OPENVR_BUILD_STATIC",
	"WIN32",
	"VRCORE_NO_PLATFORM",
	"_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING"

}

includedirs{
    	"./src",
    	"./headers",
	"./src/vrcore/",
	"./src/json/"
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
	"**.c",
	
	"**.hlsl",
    "**.hlsli"
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
