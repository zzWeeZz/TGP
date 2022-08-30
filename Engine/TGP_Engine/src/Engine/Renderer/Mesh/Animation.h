#pragma once
#include <filesystem>
#include <string>
#include "ToolBox/Math/Vector/Vector.h"
#include "ToolBox/Math/Matrix/Matrix.h"

struct Animation
{
	struct Frame
	{
		std::vector<Matrix4x4f> localTransforms;
	};

	std::vector<Frame> frames;

	uint32_t length = 0;
	float duration = 0.0f;
	float fps = 0;
	std::filesystem::path path;
};