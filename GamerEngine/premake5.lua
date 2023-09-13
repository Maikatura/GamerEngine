include "dependencies.lua"

workspace "GamerEngine"
    architecture "x64"
    startproject "Editor"
    configurations
    {
        "Debug",
        "Release",
		"Distribution"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Application"
include "Editor"
include "Modelviewer"

group "External"
include "External/CommonUtilities"
include "External/TurNet"
include "External/FBXImporter"
include "External/ImGui"
include "External/yaml-cpp"
include "External/WICTextureLoader"
include "External/rapidjson"
include "External/cryptopp"
include "External/PyBullet"
include "External/OpenCL"
include "External/flecs"
include "External/OpenFBX"
include "External/OpenVR"
	
group "Backend"
include "Shaders"
include "AudioEngine"
include "PhysicsEngine"

group "Engine"
include "Game"
include "Engine"

group "Server"
include "Server"
include "GameServer"
