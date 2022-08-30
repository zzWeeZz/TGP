#pragma once
#include "shader.h"

namespace Engine
{
	class GeometryShader : public Shader
	{
	public:
		GeometryShader(const std::filesystem::path& filePath);

		void Bind() override;
		void UnBind() override;

		static Ref<GeometryShader> Create(const std::filesystem::path& filePath);

	private:
		ComPtr<ID3D11GeometryShader> myShader;
		ComPtr<ID3D10Blob> myBuffer;
	};
}