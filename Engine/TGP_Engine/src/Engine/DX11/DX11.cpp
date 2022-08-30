#include "DX11.h"
#include <cassert>
#include <DirectXColors.h>
#include "Application/Application.h"

Engine::DX11 Engine::DX11::myInstance;
Engine::DX11::DX11()
{
	myInstance = *this;
}
#pragma region Getters
ComPtr<IDXGISwapChain>& Engine::DX11::SwapChain()
{
	return myInstance.mySwapchain;
}

ComPtr<ID3D11Device>& Engine::DX11::Device()
{
	return myInstance.myDevice;
}

ComPtr<ID3D11DeviceContext>& Engine::DX11::Context()
{
	return myInstance.myDeviceContext;
}

ComPtr<ID3D11RenderTargetView>& Engine::DX11::SwapChainRenderView()
{
	return myInstance.mySwapchainRenderTargetView;
}

ComPtr<ID3D11DepthStencilView>& Engine::DX11::SwapChainDepthView()
{
	return myInstance.mySwapchainDepthStencilView;
}

Engine::RenderStateManager& Engine::DX11::GetRenderStateManager()
{
	return myInstance.myRenderStateManager;
}
#pragma endregion

void Engine::DX11::Initialize(int32_t width, int32_t height, bool fullscreen, bool enableDebug)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = Application::GetWindow()->GetHwnd();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = !fullscreen;

	auto hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		enableDebug ? D3D11_CREATE_DEVICE_DEBUG : NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		DX11::SwapChain().GetAddressOf(),
		DX11::Device().GetAddressOf(),
		NULL,
		DX11::Context().GetAddressOf());
	assert(SUCCEEDED(hr));
	ID3D11Texture2D* backBuffer;
	hr = myInstance.mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	assert(SUCCEEDED(hr));
	hr = myInstance.myDevice->CreateRenderTargetView(backBuffer, NULL, &myInstance.mySwapchainRenderTargetView);
	assert(SUCCEEDED(hr));
	backBuffer->Release();
	backBuffer = nullptr;

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.MiscFlags = 0;

	hr = myInstance.myDevice->CreateTexture2D(&depthStencilDesc, NULL, &myInstance.mySwapchainDepthStencilBuffer);
	assert(SUCCEEDED(hr));
	hr = myInstance.myDevice->CreateDepthStencilView(myInstance.mySwapchainDepthStencilBuffer.Get(), NULL, &myInstance.mySwapchainDepthStencilView);
	assert(SUCCEEDED(hr));

	myInstance.myDeviceContext->OMSetRenderTargets(1, myInstance.mySwapchainRenderTargetView.GetAddressOf(), myInstance.mySwapchainDepthStencilView.Get());

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Height = static_cast<FLOAT>(height);
	viewport.Width = static_cast<FLOAT>(width);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	DX11::Context()->RSSetViewports(1, &viewport);
	myInstance.myRenderStateManager.Initialize();
	myInstance.myRenderStateManager.PushRasterizerState(CullMode::Back);
	myInstance.myRenderStateManager.PushBlendState(BlendMode::Opaque);
	myInstance.myRenderStateManager.PushDepthStencilState(DepthStencilMode::ReadWrite);
}

void Engine::DX11::Resize(int width, int height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	assert(myInstance.myDeviceContext);
	assert(myInstance.myDevice);
	assert(myInstance.mySwapchain);

	myInstance.mySwapchainDepthStencilBuffer.Reset();
	myInstance.mySwapchainDepthStencilView.Reset();
	myInstance.mySwapchainRenderTargetView.Reset();

	ID3D11RenderTargetView* nullViews[] = { nullptr, nullptr };
	myInstance.myDeviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	myInstance.myDeviceContext->Flush();
	//Resize swap chain
	auto hr = myInstance.mySwapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);
	assert(SUCCEEDED(hr));

	////Create new views
	ID3D11Texture2D* backBuffer;
	hr = myInstance.mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	assert(SUCCEEDED(hr));
	hr = myInstance.myDevice->CreateRenderTargetView(backBuffer, 0, &myInstance.mySwapchainRenderTargetView);
	assert(SUCCEEDED(hr));
	backBuffer->Release();
	//Create depth stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.MiscFlags = 0;


	hr = myInstance.myDevice->CreateTexture2D(&depthStencilDesc, 0, myInstance.mySwapchainDepthStencilBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = myInstance.myDevice->CreateDepthStencilView(myInstance.mySwapchainDepthStencilBuffer.Get(), 0, myInstance.mySwapchainDepthStencilView.GetAddressOf());
	assert(SUCCEEDED(hr));
	myInstance.myDeviceContext->OMSetRenderTargets(1, myInstance.mySwapchainRenderTargetView.GetAddressOf(), myInstance.mySwapchainDepthStencilView.Get());
	//Bind to output merger

	//Set viewport
	D3D11_VIEWPORT viewportDesc = {};
	viewportDesc.TopLeftX = 0.f;
	viewportDesc.TopLeftY = 0.f;
	viewportDesc.Width = static_cast<float>(width);
	viewportDesc.Height = static_cast<float>(height);
	viewportDesc.MinDepth = 0.f;
	viewportDesc.MaxDepth = 1.f;

	myInstance.myDeviceContext->RSSetViewports(1, &viewportDesc);
}

void Engine::DX11::ClearView()
{
	myInstance.myDeviceContext->ClearRenderTargetView(myInstance.mySwapchainRenderTargetView.Get(), DirectX::Colors::DarkGray);
	myInstance.myDeviceContext->ClearDepthStencilView(myInstance.mySwapchainDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Engine::DX11::Bind()
{
	myInstance.myDeviceContext->OMSetRenderTargets(1, myInstance.mySwapchainRenderTargetView.GetAddressOf(), myInstance.mySwapchainDepthStencilView.Get());
}

void Engine::DX11::Present(const bool& vsync)
{
	myInstance.mySwapchain->Present(vsync, 0);
}

void Engine::DX11::CleanUpDX11()
{
	myInstance.mySwapchain.Reset();
	myInstance.myDevice.Reset();
	myInstance.myDeviceContext.Reset();
	myInstance.mySwapchainRenderTargetView.Reset();
	myInstance.mySwapchainDepthStencilBuffer.Reset();
	myInstance.mySwapchainDepthStencilView.Reset();
}
