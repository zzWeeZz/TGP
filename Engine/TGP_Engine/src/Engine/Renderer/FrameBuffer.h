#pragma once
#include "Engine/DX11/DX11.h"
#include "ToolBox/Math/Vector/Vector.h"
namespace Engine
{
	struct FrameBufferSpecs
	{
		uint32_t width, height;
		uint32_t samples = 1;
		std::vector<ImageFormat> formats;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FrameBufferSpecs& specs);
		void Resize(const Vector2f& size);
		void Bind();
		void BindToShader(uint32_t slot, uint32_t resourceView);
		void BindDepthToShader(uint32_t slot);
		void TransferDepth(Ref<FrameBuffer> fb);
		void UnBind();
		void Clear(const Vector4f& color =  { 0.1, 0.1,0.1, 1});
		ComPtr<ID3D11ShaderResourceView> GetColorAttachment(uint32_t index) { return m_ShaderResourceViews[index].Get(); };
		inline FrameBufferSpecs& GetSpecs() { return m_Data; }

		static Ref<FrameBuffer> Create(const FrameBufferSpecs& specs);
	private:
		void Invalidate();

		std::vector<ComPtr<ID3D11Texture2D>> m_ColorBuffers;
		std::vector<ComPtr<ID3D11RenderTargetView>> m_RenderTargetViews;
		std::vector<ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;
		ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		ComPtr<ID3D11ShaderResourceView> m_DepthShaderResource;
		ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		D3D11_VIEWPORT m_Viewport = {};
		FrameBufferSpecs m_Data;

	};
}