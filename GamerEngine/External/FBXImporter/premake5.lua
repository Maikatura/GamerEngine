project "FBXImporter"
location "."
kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "Off"

targetdir ("%{wks.location}/Temp/lib/")
objdir ("%{wks.location}/Temp/Intermediate/%{cfg.buildcfg}/%{prj.name}")
targetname("%{cfg.buildcfg}/%{prj.name}")

pchheader "TGAFbx.pch.h"
pchsource "TGAFbx.pch.cpp"

linkoptions
{
    "-ignore:4006",
    "-ignore:4099",
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

flags
{
    "MultiProcessorCompile"
}

files {
	"**.h",
	"**.cpp",
	"**.hpp",
    "**.inl"
}

externalincludedirs {
    "%{IncludeDirs.FBXSDK}"
}

externalwarnings "Off"

defines{
    "FBXSDK_SHARED"
}

libdirs {
}

filter "system:windows"
    systemversion "latest"


filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "On"
    links {
    }

filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "On"
    links {
    }

filter "configurations:Distribution"
    defines "_DISTRIBUTION"
    runtime "Release"
    optimize "On"
	symbols "Off"
    links {
    }

filter{}
