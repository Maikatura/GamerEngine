project "Gamer-ScriptCore"
location "."
kind "SharedLib"
language "C#"
dotnetframework "4.7.2"

targetdir ("%{wks.location}/bin/Resources/Scripts")
objdir ("%{wks.location}/Temp/Resources/Scripts/Intermediates")

files 
{
	"%{prj.name}/src/**.cs",
	"%{prj.name}/properties/**.cs"
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
