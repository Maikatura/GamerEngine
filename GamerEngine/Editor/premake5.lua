project "Editor"
location "."
kind "WindowedApp"
language "C++"
cppdialect "C++20"
staticruntime "Off"
debugdir "%{wks.location}/Bin"
configurations { Debug, Release}
configmap {
    ["Debug"] = "Debug",
    ["Release"] = "Release",
    ["Distribution"] = "Release"
}
targetdir ("%{wks.location}/Bin/")
objdir ("%{wks.location}/Temp/Intermediate/%{cfg.buildcfg}/%{prj.name}")
targetname("%{prj.name}_%{cfg.buildcfg}")

pchheader "Editor.pch.h"
pchsource "src/Editor.pch.cpp"

linkoptions
{
    "-ignore:4006",
    "-ignore:4099",
    "-ignore:4324",
	"-ignore:4251",
	"-ignore:4275",
	"-ignore:4355",
	"-ignore:4505",
	"-ignore:4244"
}

disablewarnings
{
    "26812", --prefer enum class over enum
    "26451", --arithmetic overflow
}

defines
{
	"%{ProjectDefine.Platform}",
	"%{ProjectDefine.Backend}",
	"%{ProjectDefine.Editor}",
	"%{ProjectDefine.Network}",

    	"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"_WINSOCKAPI_",
	"NOMINMAX",
	"_CRT_SECURE_NO_WARNINGS",
	"_HAS_EXCEPTIONS=0",
	"WITH_PROFILING=1"
}

libdirs 
{ 
	"%{wks.location}/Temp/lib/"
}

includedirs{
    	"./src",
	"%{IncludeDirs.RapidJson}",
	"%{IncludeDirs.ImGui}",
	"%{IncludeDirs.GraphicsEngine}",
	"%{IncludeDirs.CommonUtilities}",
	"%{IncludeDirs.YamlCpp}",
	"%{IncludeDirs.ImGuiOnly}",
	"%{IncludeDirs.Entt}",
	"%{IncludeDirs.Json}",
	"%{IncludeDirs.TurNet}",
	"%{IncludeDirs.WICTextureLoader}",
	"%{IncludeDirs.FBXImporter}",
	"%{IncludeDirs.OpenFBX}",
	"%{IncludeDirs.OpenVR}",
    	"%{IncludeDirs.FBXSDK}",
	"%{IncludeDirs.Physics}",
	--"%{IncludeDirs.PhysX}",
	"%{IncludeDirs.Flecs}",
	"%{IncludeDirs.mono}"
}

externalincludedirs {

}

externalwarnings "Off"

dependson "Gamer-ScriptCore"

links
{
	"FBXImporter",
	"Engine",
	"Physics",
	"Flecs",
	
	--"PhysX",
	
	"Json",
	"YamlCpp",
	"OpenFBX",
	"OpenVR",
	
	"ImGui",
	"msdfgen",
	"msdf-atlas-gen",
	"freetype",
	
	--"cryptlib",
	"TurNet",
	
	"d3d11.lib",
	"dxguid.lib",
	"opengl32.lib",
	"%{Libs.OpenVR}",
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
	"**.rc",
	"**.ico",
	"**.APS"
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
	links { 
        "%{Libs.FBXSDK_DEBUG}"
    }
	
filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "On"
	links { 
        "%{Libs.FBXSDK_RELEASE}"
    }
    
filter {}
