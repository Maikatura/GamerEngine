#pragma once

#include "PhysicsTypes.h"

#include "Math/MathTypes.hpp"

namespace GamerEngine {

	enum class PhysicsDebugType
	{
		DebugToFile = 0,
		LiveDebug
	};

	struct PhysicsSettings
	{
		float FixedTimestep = 1.0f / 60.0f;
		Vector3f Gravity = { 0.0f, -9.81f, 0.0f };
		uint32_t PositionSolverIterations = 2;
		uint32_t VelocitySolverIterations = 10;

		uint32_t MaxBodies = 5700;

		bool CaptureOnPlay = true;
		PhysicsDebugType CaptureMethod = PhysicsDebugType::DebugToFile;
	};

}
