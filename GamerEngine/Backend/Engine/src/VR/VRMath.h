#pragma once
#include "GraphicsEngine.pch.h"
#include "openvr.h"
#include "Math/MathTypes.hpp"

inline Matrix4x4f ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& matPose)
{
	Matrix4x4f matrixObj;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			matrixObj(row + 1, col + 1) = matPose.m[col][row]; // Transpose the matrix during conversion
		}
	}

	return matrixObj;
}

inline Matrix4x4f ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix44_t& matPose)
{
	Matrix4x4f matrixObj;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			matrixObj(row + 1, col + 1) = matPose.m[col][row]; // Transpose the matrix during conversion
		}
	}

	return matrixObj;
}