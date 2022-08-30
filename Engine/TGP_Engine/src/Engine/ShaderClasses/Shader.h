#pragma once
#include <d3d11.h>
#include <filesystem>
#include "Engine/Utils/Utilities.h"
namespace Engine
{
	class Shader
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual ~Shader() {}
	};
}