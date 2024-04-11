#include "GraphicsEngine.pch.h"
#include "VRSystem.h"

#include "VRMath.h"
#include "Types\EnumTypes.h"

bool VRSystem::Init(HWND aWindowHandle)
{
	

#if ENABLE_VR

	


	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;

	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, ARRAYSIZE(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		std::string temp(buf);
		std::wstring wtemp(temp.begin(), temp.end());
		MessageBox(aWindowHandle, wtemp.c_str(), L"VR_Init Failed", 0);
		return false;
	}

	m_pHMD->GetRecommendedRenderTargetSize(&myRenderWidth, &myRenderHeight);

	printf("width = %d, height = %d", myRenderWidth, myRenderHeight);

	//m_nRenderWidth /= 2;
	//m_nRenderHeight /= 4;

	//clientWidth = m_nRenderWidth;
	//clientHeight = m_nRenderHeight;

	m_pRenderModels = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!m_pRenderModels)
	{
		m_pHMD = NULL;
		vr::VR_Shutdown();

		char buf[1024];
		sprintf_s(buf, ARRAYSIZE(buf), "Unable to get render model interface: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		std::string temp(buf);
		std::wstring wtemp(temp.begin(), temp.end());
		MessageBox(aWindowHandle, wtemp.c_str(), L"VR_Init Failed", NULL);
		return false;
	}

	if (!vr::VRCompositor())
	{
		printf("Compositor initialization failed. See log file for details\n");
		return false;
	}

#endif

	return true;
}

void VRSystem::Update()
{
	if (!m_pHMD)
		return;

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

	m_iValidPoseCount = 0;
	m_strPoseClasses = "";
	for (uint32_t nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			m_iValidPoseCount++;
			m_rmat4DevicePose[nDevice] = ConvertSteamVRMatrixToMatrix4(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (m_rDevClassChar[nDevice] == 0)
			{
				switch (m_pHMD->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C';	break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H';	break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I';	break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'O';	break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T';	break;
				case vr::TrackedDeviceClass_DisplayRedirect:									break;
				case vr::TrackedDeviceClass_Max:												break;
				default:                                       m_rDevClassChar[nDevice] = '?';	break;
				}
			}
			m_strPoseClasses += m_rDevClassChar[nDevice];
		}
	}

	if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		m_mat4HMDPose = Matrix4x4f::GetFastInverse(m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd]);
	}
	else
	{
		printf("pose not valid");
	}

}

uint32_t VRSystem::GetWidth() const
{
	return myRenderWidth;
}

uint32_t VRSystem::GetHeight() const
{
	return myRenderHeight;
}

void VRSystem::SetWidth(uint32_t aWidth)
{
	myRenderWidth = aWidth;
}

void VRSystem::SetHeight(uint32_t aHeight)
{
	myRenderHeight = aHeight;
}

bool VRSystem::IsVrNull() const
{
	if (m_pHMD)
	{
		return true;
	}

	return false;
}

Matrix4x4f VRSystem::GetEyeToHeadTransform(VREye anEye) const
{
	return ConvertSteamVRMatrixToMatrix4(m_pHMD->GetEyeToHeadTransform(VREye::Left == anEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right));
}

Matrix4x4f VRSystem::GetProjectionMatrix(VREye anEye, float aNearPlane, float aFarPlane) const
{
	return ConvertSteamVRMatrixToMatrix4(m_pHMD->GetProjectionMatrix(VREye::Left == anEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right, aNearPlane, aFarPlane));
}

Matrix4x4f VRSystem::GetHMDPose()
{
	return m_mat4HMDPose;
}

