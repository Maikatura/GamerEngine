include "Premake/extensions.lua"

workspace "GamerEngine"
	location "."
	startproject "Engine" -- proably launcher
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Retail"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Tga2D"

include "GamerEngine/Engine"
include "GamerEngine/CommonUtilities"
include "GamerEngine/External"

group "Dependencies"
include "Source/CommonUtilities"

group ""
include "Source/Game"