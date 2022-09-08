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
		template<typename T>
		T ReadPixel(uint32_t index, uint32_t x, uint32_t y);
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

	template<typename T>
	inline T FrameBuffer::ReadPixel(uint32_t index, uint32_t x, uint32_t y)
	{
		assert(x >= 0 && x <= (uint32_t)m_Data.width);
		assert(y >= 0 && y <= (uint32_t)m_Data.height);

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = ((uint32_t)m_Data.width);
		desc.Height = ((uint32_t)m_Data.height);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = FormatToDXFormat(m_Data.formats[index]);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		HRESULT result;
		ID3D11Texture2D* texture;
		result = DX11::Device()->CreateTexture2D(&desc, nullptr, &texture);
		assert(SUCCEEDED(result));

		auto context = DX11::Context();
		context->CopyResource(texture, m_ColorBuffers[index].Get());

		D3D11_MAPPED_SUBRESOURCE subresource = {};

		context->Map(texture, 0, D3D11_MAP_READ, 0, &subresource);

		T* data = reinterpret_cast<T*>(subresource.pData);

		uint32_t loc = (x + y * subresource.RowPitch / sizeof(T));
		T value = data[loc];

		context->Unmap(texture, 0);

		texture->Release();
		texture = nullptr;

		return value;
	}
}