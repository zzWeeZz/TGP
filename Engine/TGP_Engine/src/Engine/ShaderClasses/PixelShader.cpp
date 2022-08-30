#include "PixelShader.h"

#include <d3dcompiler.h>


Engine::PixelShader::PixelShader(const std::filesystem::path& filePath)
{
	AssertIfFailed(D3DReadFileToBlob(filePath.c_str(), myBuffer.GetAddressOf()));
	AssertIfFailed(DX11::Device()->CreatePixelShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf()));	
}

void Engine::PixelShader::Bind()
{
	DX11::Context()->PSSetShader(myShader.Get(), nullptr, 0);
}

void Engine::PixelShader::UnBind()
{
	DX11::Context()->PSSetShader(nullptr, nullptr, 0);
}

Engine::Ref<Engine::PixelShader> Engine::PixelShader::Create(const std::filesystem::path& filePath)
{
	return CreateRef<PixelShader>(filePath);
}
