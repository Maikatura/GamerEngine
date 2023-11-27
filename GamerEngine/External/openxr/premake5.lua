project "OpenXR"
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
        "_SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING",
	"XR_OS_WINDOWS"

}

includedirs{
   	
	"./include",
	"./src/external/jsoncpp/include",
	"./src/external/jsoncpp/src",

	"./src/external/jnipp",
	"./src/loader",
	"./src/common",
	"./src/external/android-jni-wrappers/wrap",
	
	"C:/Program Files/Java/jdk-21/include",
	"C:/Program Files/Java/jdk-21/include/win32",
	"$(JAVA_HOME)/include",
	"$(JAVA_HOME)/include/win32"
	
}

externalincludedirs {
	
}

externalwarnings "Off"

links
{
	"Kernel32.lib"
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
