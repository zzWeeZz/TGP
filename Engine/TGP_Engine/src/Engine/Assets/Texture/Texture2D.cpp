#include "Texture2D.h"
#include "Engine/DX11/DX11.h"
#include "Engine/DX11/DDSTextureLoader11.h"

Engine::Texture2D::Texture2D(const std::filesystem::path& path, const bool& isSRGB)
{
	if(!std::filesystem::exists(path))
	{
		std::cout << "Texture path: " << path << ". Does not exist!\n";
	}
	if (path.extension().string() == ".dds")
	{
		auto device = DX11::Device();
		ID3D11Resource* resource = nullptr;
		DirectX::CreateDDSTextureFromFile(device.Get(), path.wstring().c_str(), &resource, m_ShaderResourceView.GetAddressOf());

		AssertIfFailed(resource->QueryInterface(IID_ID3D11Texture2D, reinterpret_cast<void**>(m_Texture.GetAddressOf())));

		D3D11_TEXTURE2D_DESC desc;
		m_Texture->GetDesc(&desc);

		m_Width = desc.Width;
		m_Height = desc.Height;
	}
	else
	{
		std::cout << "Texture2D: Unsupported file format!" << std::endl;
	}
}

void Engine::Texture2D::Bind(uint32_t slot, ShaderType shaderType)
{
	switch (shaderType)
	{
	case ShaderType::Vertex: DX11::Context()->VSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf()); break;
	case ShaderType::Pixel: DX11::Context()->PSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf()); break;
	case ShaderType::Geometry: DX11::Context()->GSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf()); break;
	case ShaderType::Hull: DX11::Context()->HSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf()); break;
	case ShaderType::Domain: DX11::Context()->DSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf()); break;
	case ShaderType::Compute: DX11::Context()->CSSetShaderResources(slot, 1, m_ShaderResourceView.GetAddressOf()); break;
	}
}


void Engine::Texture2D::UnBind(uint32_t slot, ShaderType shaderType)
{
	switch (shaderType)
	{
	case ShaderType::Vertex: DX11::Context()->VSSetShaderResources(slot, 1, nullptr); break;
	case ShaderType::Pixel: DX11::Context()->PSSetShaderResources(slot, 1, nullptr); break;
	case ShaderType::Geometry: DX11::Context()->GSSetShaderResources(slot, 1, nullptr); break;
	case ShaderType::Hull: DX11::Context()->HSSetShaderResources(slot, 1, nullptr); break;
	case ShaderType::Domain: DX11::Context()->DSSetShaderResources(slot, 1, nullptr); break;
	case ShaderType::Compute: DX11::Context()->CSSetShaderResources(slot, 1, nullptr); break;
	}
}

Engine::Ref<Engine::Texture2D> Engine::Texture2D::Create(const std::filesystem::path& aPath, const bool& isSRGB)
{
	return CreateRef<Texture2D>(aPath, isSRGB);
}
