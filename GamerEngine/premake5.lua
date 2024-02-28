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
include "Runtime"

group "External"
include "External/CommonUtilities"
include "External/TurNet"
include "External/FBXImporter"
include "External/ImGui"
include "External/yaml-cpp"
include "External/WICTextureLoader"
include "External/Json"
--include "External/PhysX"
include "External/OpenCL"
include "External/flecs"
include "External/OpenFBX"
include "External/OpenVR"
include "External/msdf"
include "External/msdf/msdfgen"
--include "External/openxr"
include "External/JoltPhysics/JoltPhysicsPremake"
include "External/JoltPhysics/JoltViewerPremake"
	
group "Backend"
include "Shaders"
include "AudioEngine"
include "PhysicsEngine"


group "Engine"
include "Game"
include "Engine"

group "Server"
--include "Server"
