#include "GeometryShader.h"

#include <d3dcompiler.h>

#include "Engine/DX11/DX11.h"

Engine::GeometryShader::GeometryShader(const std::filesystem::path& filePath)
{
	AssertIfFailed(D3DReadFileToBlob(filePath.c_str(), myBuffer.GetAddressOf()));
	AssertIfFailed(DX11::Device()->CreateGeometryShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf()));
}

void Engine::GeometryShader::Bind()
{
	DX11::Context()->GSSetShader(myShader.Get(), nullptr, 0);
}

void Engine::GeometryShader::UnBind()
{
	DX11::Context()->GSSetShader(nullptr, nullptr, 0);
}

Engine::Ref<Engine::GeometryShader> Engine::GeometryShader::Create(const std::filesystem::path& filePath)
{
	return CreateRef<GeometryShader>(filePath);
}	