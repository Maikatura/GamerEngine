project "Shaders"
location "."
kind "WindowedApp"
language "C++"
cppdialect "C++20"
staticruntime "Off"
debugdir "%{wks.location}/bin"
configurations { Debug, Release }
configmap {
    ["Debug"] = "Debug",
    ["Release"] = "Release",
    ["Distribution"] = "Release"
}
targetdir ("%{wks.location}/bin/Shaders")
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


externalwarnings "Off"

links
{
}

files 
{
	"**.h",
	"**.cpp",
	"**.hpp",
	
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
	
    
	filter { "files:**.hlsli" }
    flags "ExcludeFromBuild"
    shadermodel "5.0"
 
filter { "files:**.hlsl" }
    flags "ExcludeFromBuild"
    shadermodel "5.0"

filter { "files:**.hlsl", "configurations:Debug" }
    shaderdefines { "_DEBUG" }

filter { "files:**.hlsl", "configurations:Release" }
    shaderdefines { "_DEBUG" }

 filter { "files:**PS.hlsl" }
    removeflags "ExcludeFromBuild"
    shadertype "Pixel"
    shaderentry "main"
 
 filter { "files:**VS.hlsl" }
    removeflags "ExcludeFromBuild"
    shadertype "Vertex"
    shaderentry "main"
    
 filter { "files:**GS.hlsl" }
    removeflags "ExcludeFromBuild"
    shadertype "Geometry"
    shaderentry "main"

filter { "files:**CS.hlsl" }
    removeflags "ExcludeFromBuild"
    shadertype "Compute"
    shaderentry "main"
	
filter {}
