#pragma once
#include <d3d11.h>
#include <memory>
#include "Engine/DX11/DX11.h"
#include "Engine/Utils/Utilities.h"

namespace Engine
{
	template<class T>
	class ConstantBuffer
	{
	public:
		void Create();
		void SetData(const T* data);
		ID3D11Buffer* GetBuffer() { return myBuffer; }
		void Bind(uint8_t slot, bool compute = false);
	private:
		ComPtr<ID3D11Buffer> myBuffer;
	};

	template<class T>
	void ConstantBuffer<T>::Create()
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		DX11::Device()->CreateBuffer(&bufferDesc, NULL, myBuffer.GetAddressOf());
	}

	template <class T>
	void ConstantBuffer<T>::SetData(const T* data)
	{
		D3D11_MAPPED_SUBRESOURCE subResource = {};
		DX11::Context()->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		memcpy(subResource.pData, data, sizeof(T));
		DX11::Context()->Unmap(myBuffer.Get(), 0);
	}

	template<class T>
	inline void ConstantBuffer<T>::Bind(uint8_t slot, bool compute)
	{
		if (!compute)
		{
			DX11::Context()->VSSetConstantBuffers(slot, 1, myBuffer.GetAddressOf());
			DX11::Context()->PSSetConstantBuffers(slot, 1, myBuffer.GetAddressOf());
			DX11::Context()->GSSetConstantBuffers(slot, 1, myBuffer.GetAddressOf());
		}
		else
		{
			DX11::Context()->CSSetConstantBuffers(slot, 1, myBuffer.GetAddressOf());
		}
	}
}