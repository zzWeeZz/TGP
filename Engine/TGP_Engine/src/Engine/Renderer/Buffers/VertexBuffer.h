#pragma once
#include <d3d11.h>
#include "Engine/Utils/Utilities.h"
#include "Engine/DX11/DX11.h"

namespace Engine
{
	template<class T>
	class VertexBuffer
	{
	public:
		VertexBuffer() = default;

		VertexBuffer(const VertexBuffer<T>& aBuffer);

		VertexBuffer<T>& operator=(const VertexBuffer<T>& a);

		ID3D11Buffer* Get() const { return myBuffer.Get(); }

		ID3D11Buffer* const* GetAddressOf() const { return myBuffer.GetAddressOf(); }

		UINT GetBufferSize() const { return myBufferSize; }

		const UINT Stride() const { return *myStride.get(); }

		const UINT* StridePtr() const { return myStride.get(); }

		HRESULT Initialize(T* aData, UINT aVertexCount);
		void SetData(const T* data, size_t size)
		{
			D3D11_MAPPED_SUBRESOURCE subResource = {};
			DX11::Context()->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
			memcpy_s(subResource.pData, size, data, size);
			DX11::Context()->Unmap(myBuffer.Get(), 0);
		}

		void Bind();

	private:
		ComPtr<ID3D11Buffer> myBuffer;
		std::shared_ptr<UINT> myStride;
		UINT myBufferSize = 0;
	};
}

template <class T>
Engine::VertexBuffer<T>::VertexBuffer(const VertexBuffer<T>& aBuffer)
{
	myBuffer = aBuffer.myBuffer;
	myBufferSize = aBuffer.myBufferSize;
	myStride = aBuffer.myStride;
}

template<class T>
inline Engine::VertexBuffer<T>& Engine::VertexBuffer<T>::operator=(const VertexBuffer<T>& a)
{
	this->myBuffer = a.myBuffer;
	this->myBufferSize = a.myBufferSize;
	this->myStride = a.myStride;
	return *this;
}

template<class T>
inline HRESULT Engine::VertexBuffer<T>::Initialize(T* aData, UINT aVertexCount)
{
	myBufferSize = aVertexCount;
	myStride = std::make_shared<UINT>(sizeof(T));

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(T) * aVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	vertexBufferData.pSysMem = aData;
	auto hr = DX11::Device()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, myBuffer.GetAddressOf());
	return hr;
}

template<class T>
inline void Engine::VertexBuffer<T>::Bind()
{
	UINT offset = 0;
	DX11::Context()->IASetVertexBuffers(0, 1, myBuffer.GetAddressOf(), StridePtr(), &offset);
}
