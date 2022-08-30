#pragma once
#include "Engine/Assets/Asset.h"
#include "Engine/Assets/Texture/Texture2D.h"

namespace Engine
{
	class Material : public Asset
	{
	public:
		Material(const std::string& name);
		void SetShader(const std::string& shaderKey);
		void AddDiffuseTexture(Ref<Texture2D> texture);
		void AddNormalTexture(Ref<Texture2D> texture);
		void AddSpecularTexture(Ref<Texture2D> texture);

		void Bind();

		static Ref<Material> Create(const std::string& name);
	private:
		std::string m_ShaderKey;
		Ref<Texture2D> m_Diffuse;
		Ref<Texture2D> m_Normal;
		Ref<Texture2D> m_MaterialInfo;

		static std::unordered_map<std::string, Ref<Material>> m_Registry;
	};
}