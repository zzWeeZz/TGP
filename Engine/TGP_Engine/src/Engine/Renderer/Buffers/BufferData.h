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
		Vector4i entityId;
		Matrix4x4f modelSpace;
		Matrix4x4f bones[128];
	};


	struct DirLightData
	{
		Vector4f colorAndIntensity = { 0,0,0,0 };
		Vector4f direction;
	};
	struct DirLightBuffer
	{
		DirLightData dirLightData[8];
	};

	struct PointLightData
	{
		Vector4f colorAndIntensity = { 0,0,0,0 };
		Vector3f position = { 0,0,0 };
		float radius = 0;
	};
	struct PointLightBuffer
	{
		PointLightData  pointLightData[2000];
	};
	struct SpotLightData
	{
		Vector4f colorAndIntensity = { 0,0,0,0 };
		Vector4f position = { 0,0,0, 0 };
		Vector4f direction = { 0,0,0,0 };
		Vector4f spotInfo = { 0, 0, 0, 0 };
	};
	struct SpotLightBuffer
	{
		SpotLightData spotLightData[1000];
	};
}