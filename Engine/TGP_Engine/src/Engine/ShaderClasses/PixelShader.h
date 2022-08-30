#pragma once
#include "Shader.h"

#include "Engine/DX11/DX11.h"
#include "Engine/Utils/Utilities.h"
namespace Engine
{
	class PixelShader : public Shader
	{
	public:
		PixelShader(const std::filesystem::path& filePath);

		void Bind() override;
		void UnBind() override;

		static Ref<PixelShader> Create(const std::filesystem::path& filePath);

	private:
		ComPtr<ID3D11PixelShader> myShader;
		ComPtr<ID3D10Blob> myBuffer;
	};
}
