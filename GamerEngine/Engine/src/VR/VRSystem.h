#pragma once

#include <openvr.h>

#include "Components/CameraComponent.h"
#include "Math/MathTypes.hpp"


#define ENABLE_VR 0

class VRSystem
{
private:
	vr::IVRSystem* m_pHMD;
	vr::IVRRenderModels* m_pRenderModels;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4x4f m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4x4f m_mat4HMDPose;
	uint32_t myRenderWidth;
	uint32_t myRenderHeight;

	int m_iTrackedControllerCount;
	int m_iTrackedControllerCount_Last;
	int m_iValidPoseCount;
	int m_iValidPoseCount_Last;
	bool m_bShowCubes;

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class

public:

	bool Init(HWND aWindowHandle);

	void Update();


	uint32_t GetWidth();
	uint32_t GetHeight();

	void SetWidth(uint32_t aWidth);
	void SetHeight(uint32_t aHeight);
	bool IsVrNull();
	Matrix4x4f GetEyeToHeadTransform(VREye anEye);
	Matrix4x4f GetProjectionMatrix(VREye anEye, float aNearPlane, float aFarPlane);
	Matrix4x4f GetHMDPose();
};
