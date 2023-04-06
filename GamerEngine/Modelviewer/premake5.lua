project "Modelviewer"
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
objdir ("%{wks.location}/Temp/Intermediate/%{prj.name}")
targetname("%{prj.name}_%{cfg.buildcfg}")

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

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
}

libdirs 
{ 
	"%{wks.location}/lib/"
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
	"%{IncludeDirs.Game}",
	"%{IncludeDirs.RapidJson}",
	"%{IncludeDirs.WICTextureLoader}",
	"%{IncludeDirs.FBXImporter}",
    "%{IncludeDirs.FBXSDK}",
	"%{IncludeDirs.Flecs}",
	
	"%{IncludeDirs.TurNet}"
}

externalincludedirs {

}

externalwarnings "Off"

links
{
	"FBXImporter",
	"Engine",
	"Game",
	"RapidJson",
	"YamlCpp",
	"Flecs",
	
	"d3d11.lib",
	"dxguid.lib",
	
	"TurNet",
	"ws2_32.lib"
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
