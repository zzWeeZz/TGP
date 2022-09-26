#include "CubeBuffer.h"

namespace Engine
{
	CubeBuffer::CubeBuffer(const CubeBufferSpecs& specs)
	{
		m_Data = specs;
		m_RenderTargetViews.resize(6);
		m_ShaderResourceViews.resize(6);
		Validate();
	}
	void CubeBuffer::Resize(const Vector2f& size)
	{
		m_Data.height = size.y;
		m_Data.width = size.x;
		Validate();
	}
	void CubeBuffer::Bind()
	{
		DX11::Context()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
		DX11::Context()->RSSetViewports(1, &m_Viewport);
	}
	void CubeBuffer::BindToShader(uint32_t slot)
	{
		DX11::Context()->PSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf());
	}
	void CubeBuffer::UnBind()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		DX11::Context()->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
		DX11::Context()->Flush();
		DX11::Bind();
	}
	void CubeBuffer::Clear(const Vector4f& color)
	{
		float clr[4] = { color.x, color.y, color.z, color.w };
		DX11::Context()->ClearRenderTargetView(m_RenderTargetView.Get(), clr);
		DX11::Context()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	Ref<CubeBuffer> Engine::CubeBuffer::Create(const CubeBufferSpecs& specs)
	{
		return CreateRef<CubeBuffer>(specs);
	}

	void Engine::CubeBuffer::Validate()
	{
		if (m_RenderTargetView) m_RenderTargetView->Release();
		if (m_ShaderResourceView) m_ShaderResourceView->Release();
		if (m_TextureCube) m_TextureCube->Release();
		if (m_DepthStencilBuffer) m_DepthStencilBuffer->Release();
		if (m_DepthStencilView) m_DepthStencilView->Release();
		//if (m_DepthShaderResource) m_DepthShaderResource->Release();
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

		for (size_t i = 0; i < 6; i++)
		{

			ID3D11Texture2D* ptrSurface;
			D3D11_TEXTURE2D_DESC desc = {};
			desc.Width = m_Data.width;
			desc.Height = m_Data.height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = FormatToDXFormat(ImageFormat::Depth32);
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			auto hr = DX11::Device()->CreateTexture2D(&desc, nullptr, &ptrSurface);
			assert(SUCCEEDED(hr));
			// map ptrSurface
			m_ColorBuffers.emplace_back(ptrSurface);/*
			AssertIfFailed(DX11::Device()->CreateRenderTargetView(m_ColorBuffers[i].Get(), nullptr, m_RenderTargetViews[i].GetAddressOf()));
			AssertIfFailed(DX11::Device()->CreateShaderResourceView(m_ColorBuffers[i].Get(), nullptr, m_ShaderResourceViews[i].GetAddressOf()));*/
		}
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = m_Data.width;
		desc.Height = m_Data.height;
		desc.MipLevels = 1;
		desc.ArraySize = 6;
		desc.Format = FormatToDXFormat(ImageFormat::Depth32);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		AssertIfFailed(DX11::Device()->CreateTexture2D(&desc, nullptr, m_TextureCube.GetAddressOf()));
		D3D11_RENDER_TARGET_VIEW_DESC RTViewDesc = {};
		RTViewDesc.Format = FormatToDXFormat(ImageFormat::R32F);
		RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		RTViewDesc.Texture2DArray.ArraySize = 6;
		RTViewDesc.Texture2DArray.MipSlice = 0;
		RTViewDesc.Texture2DArray.FirstArraySlice = 0;
		AssertIfFailed(DX11::Device()->CreateRenderTargetView(m_TextureCube.Get(), &RTViewDesc, m_RenderTargetView.GetAddressOf()));
		D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc = {};
		SMViewDesc.Format = FormatToDXFormat(ImageFormat::R32F);
		SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SMViewDesc.TextureCube.MipLevels = 1;
		SMViewDesc.TextureCube.MostDetailedMip = 0;

		AssertIfFailed(DX11::Device()->CreateShaderResourceView(m_TextureCube.Get(), &SMViewDesc, m_ShaderResourceView.GetAddressOf()));

		{

			D3D11_TEXTURE2D_DESC depthStencilDesc = {};
			depthStencilDesc.Width = m_Data.width;
			depthStencilDesc.Height = m_Data.height;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 6;
			depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			depthStencilDesc.MiscFlags =0;
			AssertIfFailed(DX11::Device()->CreateTexture2D(&depthStencilDesc, nullptr, m_DepthStencilBuffer.GetAddressOf()));
			D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.ArraySize = 6;
			desc.Texture2DArray.MipSlice = 0;
			desc.Texture2DArray.FirstArraySlice = 0;

			AssertIfFailed(DX11::Device()->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &desc, m_DepthStencilView.GetAddressOf()));
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDecs{};
			srvDecs.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			srvDecs.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDecs.Texture2DArray.ArraySize = 6;
			srvDecs.Texture2DArray.FirstArraySlice = 0;
			srvDecs.Texture2DArray.MipLevels = 1;
			srvDecs.Texture2DArray.MostDetailedMip = 0;

			AssertIfFailed(DX11::Device()->CreateShaderResourceView(m_DepthStencilBuffer.Get(), &srvDecs, m_DepthShaderResource.GetAddressOf()));
		}

		m_Viewport.Width = (float)m_Data.width;
		m_Viewport.Height = (float)m_Data.height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
	}

}
