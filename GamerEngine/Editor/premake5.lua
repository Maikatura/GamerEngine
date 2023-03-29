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
objdir ("%{wks.location}/Temp/Intermediate/%{prj.name}")
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
    "26451" --arithmetic overflow
}

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"_WINSOCKAPI_",
	"NOMINMAX"
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
	"%{IncludeDirs.TurNet}",
	"%{IncludeDirs.WICTextureLoader}",
	"%{IncludeDirs.FBXImporter}",
    "%{IncludeDirs.FBXSDK}",
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
	
	"ImGui",
	
	--"cryptlib",
	"TurNet",
	"ws2_32.lib",
	
	"d3d11.lib",
	"dxguid.lib"
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
