project "Runtime"
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
    ["Distribution"] = "Distribution"
}
targetdir ("%{wks.location}/Bin/")
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

defines
{
	"GE_PLATFORM_WINDOWS",
	"GE_NETWORK",

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
	"%{IncludeDirs.WICTextureLoader}",
	"%{IncludeDirs.TurNet}",
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
	
	"msdfgen",
	"msdf-atlas-gen",
	"freetype",

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

filter "configurations:Distribution"
    defines "_Distribution"
    runtime "Release"
    optimize "On"
	links { 
        "%{Libs.FBXSDK_RELEASE}"
    }
    
filter {}
