#include "VertexShader.h"
#include <d3dcompiler.h>

Engine::VertexShader::VertexShader(const std::filesystem::path& aShaderPath, InputLayout& aInputLayout)
{
	AssertIfFailed(D3DReadFileToBlob(aShaderPath.c_str(), myBuffer.GetAddressOf()));
	AssertIfFailed(DX11::Device()->CreateVertexShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf()));
	AssertIfFailed(DX11::Device()->CreateInputLayout(aInputLayout.elements.data(), static_cast<UINT>(aInputLayout.elements.size()), myBuffer->GetBufferPointer(), myBuffer->GetBufferSize(), &myInputLayout));
}

void Engine::VertexShader::Bind()
{
	DX11::Context()->IASetInputLayout(myInputLayout.Get());
	DX11::Context()->VSSetShader(myShader.Get(), NULL, 0);
}

void Engine::VertexShader::UnBind()
{
	DX11::Context()->VSSetShader(nullptr, NULL, 0);
}

Engine::Ref<Engine::VertexShader> Engine::VertexShader::Create(const std::filesystem::path& aShaderPath, InputLayout& aInputLayout)
{
	return std::make_shared<VertexShader>(aShaderPath, aInputLayout);
}
