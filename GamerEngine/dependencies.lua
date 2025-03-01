--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/Example/Lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

--Includes
IncludeDirs = {}
IncludeDirs["GraphicsEngine"] = "%{wks.location}/Backend/Engine/src/"
IncludeDirs["Components"] = "%{wks.location}/Backend/Engine/src/"
IncludeDirs["Physics"] = "%{wks.location}/Backend/Physics/src/"

-- Eternal Includes
IncludeDirs["mono"] = "%{wks.location}/Backend/vendor/mono/include/"
IncludeDirs["Flecs"] = "%{wks.location}/External/flecs/include/"
IncludeDirs["PyBullet"] = "%{wks.location}/External/PhysX/src"
IncludeDirs["TurNet"] = "%{wks.location}/External/TurNet/"
IncludeDirs["CryptoPP"] = "%{wks.location}/External/CryptoPP/"
IncludeDirs["CommonUtilities"] = "%{wks.location}/External/CommonUtilities/src/"
IncludeDirs["FBXImporter"] = "%{wks.location}/External/FBXImporter/"
IncludeDirs["ImGui"] = "%{wks.location}/External/ImGui/src/"
IncludeDirs["ImGuiOnly"] = "%{wks.location}/External/ImGui/src/ImGui/"
IncludeDirs["Entt"] = "%{wks.location}/External/entt/"
IncludeDirs["YamlCpp"] = "%{wks.location}/External/yaml-cpp/"
IncludeDirs["WICTextureLoader"] = "%{wks.location}/External/WICTextureLoader/"
IncludeDirs["Json"] = "%{wks.location}/External/json/"
IncludeDirs["FBXSDK"] = "%{wks.location}/External/FBXImporter/ThirdParty/FBXSDK/include/"
IncludeDirs["OpenFBX"] = "%{wks.location}/External/OpenFBX/src/"
IncludeDirs["OpenVR"] = "%{wks.location}/External/openvr/headers/"
IncludeDirs["OpenXR"] = "%{wks.location}/External/openxr/include/"
IncludeDirs["DirectXTK"] = "%{wks.location}/External/directxtk/inc/"
IncludeDirs["msdf_atlas_gen"] = "%{wks.location}/External/msdf/msdf-atlas-gen/"
IncludeDirs["msdfgen"] = "%{wks.location}/External/msdf/msdfgen/"
IncludeDirs["JoltPhysics"] = "%{wks.location}/External/JoltPhysics/JoltPhysics"

--Libaries
LibDirs = {}
LibDirs["Mono"] = "%{wks.location}/backend/vendor/mono/lib/%{cfg.buildcfg}"
LibDirs["CommonUtilities"] = "%{wks.location}/External/CommonUtilities/src/"
LibDirs["FBXSDK"] = "%{wks.location}/External/FBXImporter/ThirdParty/FBXSDK/lib/"

--IncludeDirs["Example"] = "%{wks.location}/External/External/Example/include"
-- LibDirs["Example"] = "%{wks.location}/External/openvr/lib/openvr_api.lib"
--Libs["Example"] = "%{LibDirs.Example}/example.lib"

Libs = {}
Libs["mono"] = "%{LibDirs.Mono}/libmono-static-sgen.lib"
Libs["FBXSDK_DEBUG"] = "%{LibDirs.FBXSDK}/debug/libfbxsdk.lib"
Libs["FBXSDK_RELEASE"] = "%{LibDirs.FBXSDK}/release/libfbxsdk.lib"
Libs["OpenVR"] = "%{wks.location}/External/openvr/lib/openvr_api.lib"

-- Windows
Libs["WinSock"] = "Ws2_32.lib"
Libs["WinMM"] = "Winmm.lib"
Libs["WinVersion"] = "Version.lib"
Libs["BCrypt"] = "Bcrypt.lib"


--Includes
ProjectDefine = {}
ProjectDefine["Platform"] = "GE_PLATFORM_WINDOWS"
ProjectDefine["Backend"] = "GE_BACKEND_DX11" -- GE_BACKEND_DX11, GE_BACKEND_DX12, GE_BACKEND_VULKAN
ProjectDefine["Editor"] = "GE_EDITOR"
ProjectDefine["Runtime"] = "GE_RUNTIME"
ProjectDefine["Network"] = "GE_NETWORK"