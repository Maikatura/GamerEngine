project "msdf"
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
    "26451" --arithmetic overflow
}

defines
{
    "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	"MSDFGEN_USE_CPP11",
"FT2_BUILD_LIBRARY"
}

includedirs{
   	"./src",
	"./src/msdf-atlas-gen",
	"./src/msdfgen",
	"./src/msdfgen/core",
	"./src/msdfgen/ext",
	"./src/msdfgen/include",
	"./src/msdfgen/freetype/include"
}

files
	{
		"./src/msdfgen/freetype/include/ft2build.h",
		"./src/msdfgen/freetype/include/freetype/*.h",
		"./src/msdfgen/freetype/include/freetype/config/*.h",
		"./src/msdfgen/freetype/include/freetype/internal/*.h",

		"./src/msdfgen/freetype/src/autofit/autofit.c",
		"./src/msdfgen/freetype/src/base/ftbase.c",
		"./src/msdfgen/freetype/src/base/ftbbox.c",
		"./src/msdfgen/freetype/src/base/ftbdf.c",
		"./src/msdfgen/freetype/src/base/ftbitmap.c",
		"./src/msdfgen/freetype/src/base/ftcid.c",
		"./src/msdfgen/freetype/src/base/ftdebug.c",
		"./src/msdfgen/freetype/src/base/ftfstype.c",
		"./src/msdfgen/freetype/src/base/ftgasp.c",
		"./src/msdfgen/freetype/src/base/ftglyph.c",
		"./src/msdfgen/freetype/src/base/ftgxval.c",
		"./src/msdfgen/freetype/src/base/ftinit.c",
		"./src/msdfgen/freetype/src/base/ftmm.c",
		"./src/msdfgen/freetype/src/base/ftotval.c",
		"./src/msdfgen/freetype/src/base/ftpatent.c",
		"./src/msdfgen/freetype/src/base/ftpfr.c",
		"./src/msdfgen/freetype/src/base/ftstroke.c",
		"./src/msdfgen/freetype/src/base/ftsynth.c",
		"./src/msdfgen/freetype/src/base/ftsystem.c",
		"./src/msdfgen/freetype/src/base/fttype1.c",
		"./src/msdfgen/freetype/src/base/ftwinfnt.c",
		"./src/msdfgen/freetype/src/bdf/bdf.c",
		"./src/msdfgen/freetype/src/bzip2/ftbzip2.c",
		"./src/msdfgen/freetype/src/cache/ftcache.c",
		"./src/msdfgen/freetype/src/cff/cff.c",
		"./src/msdfgen/freetype/src/cid/type1cid.c",
		"./src/msdfgen/freetype/src/gzip/ftgzip.c",
		"./src/msdfgen/freetype/src/lzw/ftlzw.c",
		"./src/msdfgen/freetype/src/pcf/pcf.c",
		"./src/msdfgen/freetype/src/pfr/pfr.c",
		"./src/msdfgen/freetype/src/psaux/psaux.c",
		"./src/msdfgen/freetype/src/pshinter/pshinter.c",
		"./src/msdfgen/freetype/src/psnames/psnames.c",
		"./src/msdfgen/freetype/src/raster/raster.c",
		"./src/msdfgen/freetype/src/sdf/sdf.c",
		"./src/msdfgen/freetype/src/sfnt/sfnt.c",
		"./src/msdfgen/freetype/src/smooth/smooth.c",
		"./src/msdfgen/freetype/src/truetype/truetype.c",
		"./src/msdfgen/freetype/src/type1/type1.c",
		"./src/msdfgen/freetype/src/type42/type42.c",
		"./src/msdfgen/freetype/src/winfonts/winfnt.c"
	}

externalincludedirs {
	
}

externalwarnings "Off"

libdirs 
{ 
	
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
