#pragma once
#include <d3d11.h>
#include <stack>

#include "Engine/Utils/Utilities.h"
#include <unordered_map>

namespace Engine
{
	enum class ShaderType
	{
		Vertex = 0 << 1,
		Pixel = 1 << 1,
		Geometry = 1 << 2,
		Hull = 1 << 3,
		Domain = 1 << 4,
		Compute = 1 << 5
	};

	enum class CullMode
	{
		None,
		Front,
		Back,
		Wireframe,
	};
	enum class BlendMode
	{
		Opaque,
		AlphaBlend,
		Additive,
		AlphaAdditive,
		Subtractive,
		AlphaSubtractive,
		Multiplicative,
		AlphaMultiplicative,
		NonPremultiplied,
		AlphaNonPremultiplied,
	};
	enum class SamplerMode
	{
		Wrap,
		Mirror,
		Border,
		Point,
		Clamp,
	};

	enum class DepthStencilMode
	{
		None,
		ReadOnly,
		ReadWrite,
	};

	class RenderStateManager
	{
	public:
		void Initialize();
		void PushRasterizerState(CullMode cullMode);
		void PopRasterizerState();

		void PushBlendState(BlendMode blendMode);
		void PopBlendState();

		void PushDepthStencilState(DepthStencilMode depthStencilMode);
		void PopDepthStencilState();

		void SetSamplerState(SamplerMode samplerMode, ShaderType shaderType, uint8_t slot = 0);
	private:

		void UpdateRasterizerState();
		void UpdateBlendState();
		void UpdateDepthStencilState();

		std::unordered_map<CullMode, ComPtr<ID3D11RasterizerState>> myRasterizerStates;
		std::stack<ComPtr<ID3D11RasterizerState>> myRasterizerStateStack;

		std::unordered_map<BlendMode, ComPtr<ID3D11BlendState>> myBlendStates;
		std::stack<ComPtr<ID3D11BlendState>> myBlendStateStack;

		std::unordered_map<SamplerMode,ComPtr<ID3D11SamplerState>> mySamplerStates;

		std::unordered_map<DepthStencilMode, ComPtr<ID3D11DepthStencilState>> myDepthStencilStates;
		std::stack<ComPtr<ID3D11DepthStencilState>> myDepthStencilStateStack;
	};
}