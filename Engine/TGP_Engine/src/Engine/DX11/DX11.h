#pragma once
#include <cstdint>
#include <d3d11.h>
#include "Engine/Utils/Utilities.h"
#include "RenderStateManager.h"
namespace Engine
{
	enum class ImageFormat
	{
		RGBA32F = 2,
		RGBA32U = 3,
		RGBA32S = 4,
		RGB32F = 6,
		RGB32U = 7,
		RGB32S = 8,
		RGBA16F = 10,
		RGBA16UN = 11,
		RGBA16UI = 12,
		RGBA16SN = 13,
		RGBA16SI = 14,
		RG32F = 16,
		RG32U = 17,
		RG32S = 18,
		R8G8B8A8UN = 28,
		Depth32 = 39,
		Depth24 = 45,
		R8UN = 61,

	};

	inline DXGI_FORMAT FormatToDXFormat(const ImageFormat& format)
	{
		return static_cast<DXGI_FORMAT>(format);
	}
	inline ImageFormat DXFormatToFormat(const DXGI_FORMAT& format)
	{
		return static_cast<ImageFormat>(format);
	}

	inline bool IsDepth(ImageFormat& format)
	{
		if (format == ImageFormat::Depth24 || format == ImageFormat::Depth32)
		{
			return true;
		}
		return false;
	}

	class DX11
	{
	public:
		static ComPtr<IDXGISwapChain>& SwapChain();
		static ComPtr<ID3D11Device>& Device();
		static ComPtr<ID3D11DeviceContext>& Context();
		static ComPtr<ID3D11RenderTargetView>& SwapChainRenderView();
		static ComPtr<ID3D11DepthStencilView>& SwapChainDepthView();
		static RenderStateManager& GetRenderStateManager();

		DX11();
		static void Initialize(int32_t width, int32_t height, bool fullscreen, bool enableDebug);
		static void Resize(int width, int height);
		static void ClearView();
		static void Bind();
		static void Present(const bool& vsync);

		static void CleanUpDX11();
	private:
		static DX11 myInstance;


		RenderStateManager myRenderStateManager;
		ComPtr<IDXGISwapChain> mySwapchain;
		ComPtr<ID3D11Device> myDevice;
		ComPtr<ID3D11DeviceContext> myDeviceContext;
		ComPtr<ID3D11RenderTargetView> mySwapchainRenderTargetView;
		ComPtr<ID3D11DepthStencilView> mySwapchainDepthStencilView;
		ComPtr<ID3D11Texture2D> mySwapchainDepthStencilBuffer;
	};
}