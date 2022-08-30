#include "RenderStateManager.h"
#include "Engine/DX11/DX11.h"

void Engine::RenderStateManager::Initialize()
{
	// Rasterizer states
	D3D11_RASTERIZER_DESC cmDesc = {};
	cmDesc.FillMode = D3D11_FILL_SOLID;
	cmDesc.CullMode = D3D11_CULL_BACK;
	cmDesc.FrontCounterClockwise = false;
	DX11::Device()->CreateRasterizerState(&cmDesc, &myRasterizerStates[CullMode::Back]);
	cmDesc.CullMode = D3D11_CULL_FRONT;
	DX11::Device()->CreateRasterizerState(&cmDesc, &myRasterizerStates[CullMode::Front]);
	cmDesc.CullMode = D3D11_CULL_NONE;
	DX11::Device()->CreateRasterizerState(&cmDesc, &myRasterizerStates[CullMode::None]);
	cmDesc.FillMode = D3D11_FILL_WIREFRAME;
	DX11::Device()->CreateRasterizerState(&cmDesc, &myRasterizerStates[CullMode::Wireframe]);

	// blend states
	D3D11_BLEND_DESC blendDesc = {};
	D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	
	renderTargetBlendDesc.BlendEnable = true;
	renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
	renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
	renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = renderTargetBlendDesc;
	DX11::Device()->CreateBlendState(&blendDesc, &myBlendStates[BlendMode::AlphaBlend]);

	renderTargetBlendDesc.BlendEnable = false;
	blendDesc.RenderTarget[0] = renderTargetBlendDesc;
	DX11::Device()->CreateBlendState(&blendDesc, &myBlendStates[BlendMode::Opaque]);

	// sampler states
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX11::Device()->CreateSamplerState(&samplerDesc, &mySamplerStates[SamplerMode::Wrap]);
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	DX11::Device()->CreateSamplerState(&samplerDesc, &mySamplerStates[SamplerMode::Point]);

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	DX11::Device()->CreateSamplerState(&samplerDesc, &mySamplerStates[SamplerMode::Clamp]);

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	DX11::Device()->CreateSamplerState(&samplerDesc, &mySamplerStates[SamplerMode::Border]);

	// depth stencil states
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = false;
	DX11::Device()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[DepthStencilMode::ReadWrite]);

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DX11::Device()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[DepthStencilMode::ReadOnly]);

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	DX11::Device()->CreateDepthStencilState(&depthStencilDesc, &myDepthStencilStates[DepthStencilMode::None]);
}

void Engine::RenderStateManager::PushRasterizerState(CullMode cullMode)
{
	myRasterizerStateStack.push(myRasterizerStates[cullMode]);
	UpdateRasterizerState();
}

void Engine::RenderStateManager::PopRasterizerState()
{
	myRasterizerStateStack.pop();
	UpdateRasterizerState();
}

void Engine::RenderStateManager::PushBlendState(BlendMode blendMode)
{
	myBlendStateStack.push(myBlendStates[blendMode]);
	UpdateBlendState();
}

void Engine::RenderStateManager::PopBlendState()
{
	myBlendStateStack.pop();
	UpdateBlendState();
}

void Engine::RenderStateManager::PushDepthStencilState(DepthStencilMode depthStencilMode)
{
	myDepthStencilStateStack.push(myDepthStencilStates[depthStencilMode]);
	UpdateDepthStencilState();
}

void Engine::RenderStateManager::PopDepthStencilState()
{
	myDepthStencilStateStack.pop();
	UpdateDepthStencilState();
}

void Engine::RenderStateManager::SetSamplerState(SamplerMode samplerMode, ShaderType shaderType, uint8_t slot)
{
	switch (shaderType)
	{
	case ShaderType::Vertex:
		DX11::Context()->VSSetSamplers(slot, 1, mySamplerStates[samplerMode].GetAddressOf());
		break;
	case ShaderType::Pixel:
		DX11::Context()->PSSetSamplers(slot, 1, mySamplerStates[samplerMode].GetAddressOf());
		break;
	case ShaderType::Compute:
		DX11::Context()->CSSetSamplers(slot, 1, mySamplerStates[samplerMode].GetAddressOf());
		break;
	default:;
	}
}


void Engine::RenderStateManager::UpdateRasterizerState()
{
	DX11::Context()->RSSetState(myRasterizerStateStack.top().Get());
}

void Engine::RenderStateManager::UpdateBlendState()
{
	DX11::Context()->OMSetBlendState(myBlendStateStack.top().Get(), nullptr, 0xFFFFFFFF);
}

void Engine::RenderStateManager::UpdateDepthStencilState()
{
	DX11::Context()->OMSetDepthStencilState(myDepthStencilStateStack.top().Get(), 0);
}
