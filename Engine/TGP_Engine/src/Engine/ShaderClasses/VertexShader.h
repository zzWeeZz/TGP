#pragma once
#include "shader.h"

#include "Engine/Utils/Utilities.h"
#include "Engine/DX11/DX11.h"

namespace Engine
{
	enum class Value
	{
		Float4 = 2,
		UInt4 = 3,

		Float3 = 6,
		UInt3 = 7,

		Float2 = 16,
		UInt2 = 17,

		Float = 41,
		UInt = 42,
		Float4X4 = Float4,
		UInt4X4 = UInt4,
		Float3X3 = Float3,
		UInt3X3 = UInt3,
	};

	struct InputLayoutDefine
	{
		LPCSTR name;
		uint32_t semanticIndex;
		Value value;
	};

	struct InputLayout
	{
		InputLayout(std::initializer_list<InputLayoutDefine> defines)
		{
			for(auto& define : defines)
			{
				elements.emplace_back(define.name, define.semanticIndex, (DXGI_FORMAT)define.value, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
			}
		}
		std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
	};

	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::filesystem::path& aShaderPath, InputLayout& aInputLayout);
		
		void Bind() override;
		void UnBind() override;

		static Ref<VertexShader> Create(const std::filesystem::path& aShaderPath, InputLayout& aInputLayout);

	private:
		ComPtr<ID3D11VertexShader> myShader;
		ComPtr<ID3D10Blob> myBuffer;
		ComPtr<ID3D11InputLayout> myInputLayout;
	};
}
