project "PhysX"
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
	"4244",
	"4267"
}

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"USE_GRAPHICAL_BENCHMARK",
	"_CRT_SECURE_NO_WARNINGS",
	"_CRT_SECURE_NO_DEPRECATE",
	"_SCL_SECURE_NO_WARNINGS",
	"WIN32",
	"NDEBUG",
	"B3_USE_CLEW"
}

includedirs{
    "./src/include",
	"./src/source"
}

links
{
	"Ws2_32.lib", 
	"Winmm.lib", 
	"User32.lib", 
	"Opengl32.lib", 
	"kernel32.lib", 
	"glu32.lib", 
	"Gdi32.lib", 
	"Comdlg32.lib"
}

externalincludedirs {

}

externalwarnings "Off"

libdirs 
{ 
	"%{prj.location}/lib"
}

files 
{
	"**.h",
	"**.cpp",
	"**.hpp"
}

flags
{
    "MultiProcessorCompile"
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
