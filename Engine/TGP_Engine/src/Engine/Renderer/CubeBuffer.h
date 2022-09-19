#pragma once
#include "Engine/DX11/DX11.h"
#include "ToolBox/Math/Vector/Vector.h"
namespace Engine
{
	struct CubeBufferSpecs
	{
		uint32_t width, height;
	};
	class CubeBuffer
	{
	public:
		CubeBuffer(const CubeBufferSpecs& specs);
		void Resize(const Vector2f& size);
		void Bind();
		void BindToShader(uint32_t slot);
		void UnBind();
		void Clear(const Vector4f& color = { 0.1, 0.1,0.1, 1 });
		inline CubeBufferSpecs& GetSpecs() { return m_Data; }

		static Ref<CubeBuffer> Create(const CubeBufferSpecs& specs);
	private:
		void Validate();
		ComPtr<ID3D11Texture2D> m_TextureCube;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
		ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		std::vector<ComPtr<ID3D11Texture2D>> m_ColorBuffers;
		std::vector<ComPtr<ID3D11RenderTargetView>> m_RenderTargetViews;
		std::vector<ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;
		D3D11_VIEWPORT m_Viewport = {};
		CubeBufferSpecs m_Data;
	};
}
