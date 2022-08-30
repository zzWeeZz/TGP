#pragma once
#include <cstdint>
#include "Engine/Assets/Asset.h"
#include "Engine/DX11/RenderStateManager.h"
#define BIT(x) (1 << x)
namespace Engine
{
	class Texture
	{
	public:
		virtual void Bind(uint32_t slot, ShaderType shaderType) = 0;
		virtual void UnBind(uint32_t slot, ShaderType shaderType) = 0;
		virtual ~Texture() = default;
	};
}
