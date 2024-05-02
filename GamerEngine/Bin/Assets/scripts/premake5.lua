workspace "Sandbox"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"	
	}

	flags
	{
		"MultiProcessorCompile"	
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architechture}"

project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/bin/Resources/Scripts")
	objdir ("%{wks.location}/Temp/Resources/Scripts/Intermediates")

	files 
	{
		"./src/**.cs",
		"./properties/**.cs"
	}

    	links
	{
		"Hazel-ScriptCore"
	}
filter "configurations:Debug"
    optimize "Off"
    symbols "Off"

filter "configurations:Release"
    optimize "On"
    symbols "Default"

filter "configurations:Dist"
    optimize "Full"
    symbols "Off"
	

filter {}