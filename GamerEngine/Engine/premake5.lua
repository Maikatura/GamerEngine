project "Engine"
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


pchheader "GraphicsEngine.pch.h"
pchsource "src/GraphicsEngine.pch.cpp"


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
	"GE_PLATFORM_WINDOWS",
    	"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"NOMINMAX"
}

includedirs{
    "./src",
	"%{IncludeDirs.TurNet}",
	"%{IncludeDirs.CommonUtilities}",
	"%{IncludeDirs.FBXImporter}",
	"%{IncludeDirs.OpenFBX}",
	"%{IncludeDirs.OpenVR}",
	"%{IncludeDirs.OpenXR}",
	"%{IncludeDirs.Entt}",
	"%{IncludeDirs.Game}",
	"%{IncludeDirs.ImGui}",
	"%{IncludeDirs.YamlCpp}",
	"%{IncludeDirs.Json}",
	"%{IncludeDirs.WICTextureLoader}",
	"%{IncludeDirs.PhysX}",
	"%{IncludeDirs.DirectXTK}",
	"%{IncludeDirs.msdfgen}",
	"%{IncludeDirs.msdf_atlas_gen}",
	"%{IncludeDirs.Flecs}../include2"
	
}

externalincludedirs {
	
}

externalwarnings "Off"

links
{
	"CommonUtilities",
	"FBXImporter",
	"OpenFBX",
	"WICTextureLoader",
	"Game",
	"Shaders",
	"Flecs",
	"DirectXTK",
	"ImGui",
	"Json",
	"msdfgen",
	--"openxr"
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
