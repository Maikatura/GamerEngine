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
objdir ("%{wks.location}/Temp/Intermediate/%{cfg.buildcfg}/%{prj.name}")
targetname("%{cfg.buildcfg}/%{prj.name}")


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
	"%{ProjectDefine.Platform}",
	"%{ProjectDefine.Backend}",
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
	"%{IncludeDirs.mono}",
	"%{IncludeDirs.Game}",
	"%{IncludeDirs.ImGui}",
	"%{IncludeDirs.YamlCpp}",
	"%{IncludeDirs.Json}",
	"%{IncludeDirs.WICTextureLoader}",
	"%{IncludeDirs.PhysX}",
	"%{IncludeDirs.msdfgen}",
	"%{IncludeDirs.msdf_atlas_gen}",
	"%{IncludeDirs.Flecs}../include2",
	--"%{IncludeDirs.JoltPhysics}",
	"%{IncludeDirs.mono}"
	
}

externalincludedirs {
	
}

externalwarnings "Off"

links
{
	"d3d11.lib",
	"dxguid.lib",
	"d3dcompiler.lib",
	"CommonUtilities",
	"FBXImporter",
	"OpenFBX",
	"WICTextureLoader",
	"Shaders",
	"Flecs",
	"ImGui",
	"Json",
	"msdfgen",
	--"openxr",
	"JoltPhysics",
	"%{Libs.mono}",
	"%{Libs.WinSock}",
	"%{Libs.WinMM}",
	"%{Libs.WinVersion}",
	"%{Libs.BCrypt}"
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
