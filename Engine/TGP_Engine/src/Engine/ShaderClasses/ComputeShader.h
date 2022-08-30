#pragma once
#include "Shader.h"

namespace Engine
{
	class ComputeShader : public Shader
	{
	public:/*
		bool Initialize(const std::filesystem::path& filePath) override;
		ID3D11ComputeShader* GetShader() const { return myShader.Get(); }
		ID3D10Blob* GetBuffer() override;*/
		~ComputeShader();

	private:
		ComPtr<ID3D11ComputeShader> myShader;
	};
}