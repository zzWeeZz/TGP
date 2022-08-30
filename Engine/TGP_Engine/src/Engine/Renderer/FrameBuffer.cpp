#include "FrameBuffer.h"
#include <DirectXColors.h>
namespace Engine
{
	FrameBuffer::FrameBuffer(const FrameBufferSpecs& specs) : m_Data(specs)
	{
		m_DepthStencilBuffer = nullptr;
		m_DepthStencilView = nullptr;
		m_RenderTargetViews.resize(specs.formats.size());
		m_ShaderResourceViews.resize(specs.formats.size());
		Invalidate();
	}

	void FrameBuffer::Resize(const Vector2f& size)
	{
		m_Data.width = size.x;
		m_Data.height = size.y;
		Invalidate();
	}

	void FrameBuffer::Bind()
	{
		DX11::Context()->OMSetRenderTargets(m_RenderTargetViews.size(), m_RenderTargetViews.data()->GetAddressOf(), m_DepthStencilView.Get());
		DX11::Context()->RSSetViewports(1, &m_Viewport);
	}

	void FrameBuffer::BindToShader(uint32_t slot, uint32_t resourceView)
	{
		DX11::Context()->PSSetShaderResources(slot, 1, m_ShaderResourceViews[resourceView].GetAddressOf());
	}

	void FrameBuffer::BindDepthToShader(uint32_t slot)
	{
		DX11::Context()->PSSetShaderResources(slot, 1, m_DepthShaderResource.GetAddressOf());
	}

	void FrameBuffer::TransferDepth(Ref<FrameBuffer> fb)
	{
		m_DepthShaderResource = fb->m_DepthShaderResource.Get();
		m_DepthStencilBuffer = fb->m_DepthStencilBuffer.Get();
		m_DepthStencilView = fb->m_DepthStencilView.Get();
	}

	void FrameBuffer::UnBind()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		DX11::Context()->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
		DX11::Context()->Flush();
		DX11::Bind();
	}

	void FrameBuffer::Clear(const Vector4f& color)
	{
		float clr[4] = { color.x, color.y, color.z, color.w };
		for (auto& view : m_RenderTargetViews)
		{
			if (view)
				DX11::Context()->ClearRenderTargetView(view.Get(), clr);
		}
		if (m_DepthStencilView)
		{
			DX11::Context()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecs& specs)
	{
		return CreateRef<FrameBuffer>(specs);
	}

	void FrameBuffer::Invalidate()
	{
		if (m_DepthStencilBuffer) m_DepthStencilBuffer->Release();
		if (m_DepthStencilView) m_DepthStencilView->Release();
		if (m_DepthShaderResource) m_DepthShaderResource->Release();
		for (auto& cb : m_ColorBuffers)
		{
			if (cb) cb->Release();
		}
		m_ColorBuffers.clear();
		for (auto& rtv : m_RenderTargetViews)
		{
			if (rtv) rtv->Release();
		}
		for (auto& srv : m_ShaderResourceViews)
		{
			if (srv) srv->Release();
		}

		for (size_t i = 0; i < m_Data.formats.size(); i++)
		{
			if (!IsDepth(m_Data.formats[i]))
			{
				ID3D11Texture2D* ptrSurface;
				D3D11_TEXTURE2D_DESC desc = {};
				desc.Width = m_Data.width;
				desc.Height = m_Data.height;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Format = FormatToDXFormat(m_Data.formats[i]);
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				auto hr = DX11::Device()->CreateTexture2D(&desc, nullptr, &ptrSurface);
				assert(SUCCEEDED(hr));
				// map ptrSurface
				m_ColorBuffers.emplace_back(ptrSurface);
				AssertIfFailed(DX11::Device()->CreateRenderTargetView(m_ColorBuffers[i].Get(), nullptr, m_RenderTargetViews[i].GetAddressOf()));
				AssertIfFailed(DX11::Device()->CreateShaderResourceView(m_ColorBuffers[i].Get(), nullptr, m_ShaderResourceViews[i].GetAddressOf()));
			}
			else
			{
				D3D11_TEXTURE2D_DESC depthStencilDesc = {};
				depthStencilDesc.Width = m_Data.width;
				depthStencilDesc.Height = m_Data.height;
				depthStencilDesc.MipLevels = 1;
				depthStencilDesc.ArraySize = 1;
				depthStencilDesc.Format = FormatToDXFormat(m_Data.formats[i]);
				depthStencilDesc.SampleDesc.Count = m_Data.samples;
				depthStencilDesc.SampleDesc.Quality = 0;
				depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
				depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
				depthStencilDesc.MiscFlags = 0;
				AssertIfFailed(DX11::Device()->CreateTexture2D(&depthStencilDesc, nullptr, m_DepthStencilBuffer.GetAddressOf()));
				D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
				desc.Format = DXGI_FORMAT_D32_FLOAT;
				desc.Texture2D.MipSlice = 0;
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

				AssertIfFailed(DX11::Device()->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &desc, m_DepthStencilView.GetAddressOf()));
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDecs{};
				srvDecs.Format = DXGI_FORMAT_R32_FLOAT;
				srvDecs.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDecs.Texture2D.MipLevels = 1;
				srvDecs.Texture2D.MostDetailedMip = 0;


				AssertIfFailed(DX11::Device()->CreateShaderResourceView(m_DepthStencilBuffer.Get(), &srvDecs, m_DepthShaderResource.GetAddressOf()));
			}
		}

		m_Viewport.Width = (float)m_Data.width;
		m_Viewport.Height = (float)m_Data.height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
	}
}