project "Launcher"
	location "%{wks.location}/Local/"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	--staticruntime "on"

	targetdir ("$(SolutionDir)..\Build\")
	targetname("GamerEngine_%{cfg.buildcfg}")
	objdir ("$(SolutionDir)..\Temp\Engine\")

	debugdir "$(SolutionDir)..\Build\"

	files {
		"**.h",
		"**.cpp",
	}

	includedirs {
		"$(ProjectDir)source",
        "$(SolutionDIr)vendor\include",
		"$(SolutionDIr)vendor\include\CommonUtilities"
	}

	libdirs { 
		"$(SolutionDir)vendor\lib\gl",
		"$(SolutionDir)vendor\lib\glfw",
		"$(SolutionDir)vendor\lib\assimp",
		"$(SolutionDir)vendor\lib\GamerLib",
		"$(SolutionDir)vendor\lib\CommonUtilities" 
	}

	links { 
		"glfw3.lib",
		"glew32s.lib",
		"opengl32.lib",
		"assimp-vc142-mtd.lib",
		"External-Debug.lib",
		"CommonUtilities-d.lib"
	}

	defines {"_CONSOLE", "B2_USER_SETTINGS"}

	
	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"

		links{ "box2d_Debug.lib", "SoundEngine-FMod-d.lib"}

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

		links{"box2d_Release.lib", "SoundEngine-FMod.lib"}

	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"

		links{ "box2d_Release.lib", "SoundEngine-FMod.lib"}

	systemversion "latest"
	
	filter "system:linux"
		debugenvs { "LD_LIBRARY_PATH=../bin/Debug-linux-x86_64/" }
		defines { "TGE_SYSTEM_LINUX" }

	filter "system:windows"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"

		files { 'Launcher.rc', '**.ico' }
		
		-- sdlchecks "true"
		--conformanceMode "On"
		--buildoptions { "/STACK:8000000" }
		flags { 
		--	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		defines {
			"WIN32",
			"_LIB", 
			"TGE_SYSTEM_WINDOWS"
		}

		