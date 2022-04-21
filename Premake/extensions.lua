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

group "GamerEngine"

include "GamerEngine/Engine"


group "Dependencies"

group ""
include "Source/Game"