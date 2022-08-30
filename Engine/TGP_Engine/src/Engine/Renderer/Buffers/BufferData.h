#pragma once
#include "ToolBox/Math/Vector/Vector.h"
#include "ToolBox/Math/Matrix/Matrix.h"

namespace Engine
{
	struct CameraBuffer
	{
		Matrix4x4f cameraSpace;
		Matrix4x4f toProjectionSpace;
		Vector4f cameraPosition;
	};

	struct ModelBuffer
	{
		Matrix4x4f modelSpace;
		Matrix4x4f bones[128];
	};

	struct DirectionalLightBuffer
	{
		Vector4f colorAndIntensity;
		Vector3f direction;
		float padding;
	};
}