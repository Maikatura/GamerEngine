include "Premake/extensions.lua"

workspace "TGE"
	location "."
	startproject "Launcher" -- proably launcher
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Retail"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Tga2D"

include "Source/External"
include "Source/tga2dcore"
include "Source/Launcher"

group "Dependencies"
include "Source/CommonUtilities"

group ""
include "Source/Game"