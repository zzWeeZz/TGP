#pragma once
#include <d3d11.h>

#include "Texture.h"
#include "Engine/Utils/Utilities.h"

namespace Engine
{
	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::filesystem::path& path, const bool& isSRGB);

		void Bind(uint32_t slot, ShaderType shaderType) override;
		void UnBind(uint32_t slot, ShaderType shaderType) override;
		ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_ShaderResourceView; }
		static Ref<Texture2D> Create(const std::filesystem::path& aPath, const bool& isSRGB = true);
	private:
		ComPtr<ID3D11Texture2D> m_Texture;
		ComPtr<ID3D11UnorderedAccessView> m_UAV;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
		int m_Height = 0;
		int m_Width = 0;
	};
}