--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

--Includes
IncludeDirs = {}
IncludeDirs["GraphicsEngine"] = "%{wks.location}/Engine/src/"
IncludeDirs["Game"] = "%{wks.location}/Game/src"
IncludeDirs["Components"] = "%{wks.location}/Engine/src/"
IncludeDirs["PhysicsEngine"] = "%{wks.location}/PhysicsEngine/src/"

-- Eternal Includes

IncludeDirs["Flecs"] = "%{wks.location}/External/flecs/include/"
IncludeDirs["PyBullet"] = "%{wks.location}/External/PyBullet/src"
IncludeDirs["TurNet"] = "%{wks.location}/External/TurNet/"
IncludeDirs["CryptoPP"] = "%{wks.location}/External/CryptoPP/"
IncludeDirs["CommonUtilities"] = "%{wks.location}/External/CommonUtilities/src/"
IncludeDirs["FBXImporter"] = "%{wks.location}/External/FBXImporter/"
IncludeDirs["ImGui"] = "%{wks.location}/External/ImGui/src/"
IncludeDirs["ImGuiOnly"] = "%{wks.location}/External/ImGui/src/ImGui/"
IncludeDirs["Entt"] = "%{wks.location}/External/entt/"
IncludeDirs["YamlCpp"] = "%{wks.location}/External/yaml-cpp/"
IncludeDirs["WICTextureLoader"] = "%{wks.location}/External/WICTextureLoader/"
IncludeDirs["RapidJson"] = "%{wks.location}/External/rapidjson/"
IncludeDirs["FBXSDK"] = "%{wks.location}/External/FBXImporter/ThirdParty/FBXSDK/include/"
IncludeDirs["OpenFBX"] = "%{wks.location}/External/OpenFBX/include/"

--Libaries
LibDirs = {}
LibDirs["CommonUtilities"] = "%{wks.location}/External/CommonUtilities/src/"
LibDirs["FBXSDK"] = "%{wks.location}/External/FBXImporter/ThirdParty/FBXSDK/lib/"

--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

Libs = {}
Libs["FBXSDK_DEBUG"] = "%{LibDirs.FBXSDK}/debug/libfbxsdk.lib"
Libs["FBXSDK_RELEASE"] = "%{LibDirs.FBXSDK}/release/libfbxsdk.lib"