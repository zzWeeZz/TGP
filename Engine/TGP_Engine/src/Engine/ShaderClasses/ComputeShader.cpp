#include "ComputeShader.h"

#include "Engine/DX11/DX11.h"
#include <cassert>
#include <d3dcompiler.h>

//
//bool Engine::ComputeShader::Initialize(const std::filesystem::path& filePath)
//{
//	auto hr = D3DReadFileToBlob(filePath.c_str(), myBuffer.GetAddressOf());
//	assert(SUCCEEDED(hr));
//
//	hr = DX11::Device()->CreateComputeShader(myBuffer.Get()->GetBufferPointer(), myBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf());
//	assert(SUCCEEDED(hr));
//	return true;
//}
//
//ID3D10Blob* Engine::ComputeShader::GetBuffer()
//{
//	return myBuffer.Get();
//}
//
//
//Engine::ComputeShader::~ComputeShader()
//{
//	myShader.Reset();
//	myBuffer.Reset();
//}