#include "IndexBuffer.h"
#include "Engine/DX11/DX11.h"

HRESULT Engine::IndexBuffer::Initalize(DWORD* aData, UINT aNumberOfIndices)
{
	myBufferSize = aNumberOfIndices;

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * aNumberOfIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = aData;
	auto hr = DX11::Device()->CreateBuffer(&indexBufferDesc, &iinitData, &myIndexsBuffer);
	return hr;
}

ID3D11Buffer* Engine::IndexBuffer::Get() const
{
	return myIndexsBuffer.Get();
}

ID3D11Buffer* const* Engine::IndexBuffer::GetAddressOf() const
{
	return myIndexsBuffer.GetAddressOf();
}

UINT Engine::IndexBuffer::BufferSize() const
{
	return myBufferSize;
}
