#include "Material.h"

#include "Engine/ShaderClasses/ShaderLibrary.h"

std::unordered_map<std::string, Engine::Ref<Engine::Material>> Engine::Material::m_Registry;
Engine::Material::Material(const std::string& name)
{
	m_Diffuse = Texture2D::Create("Assets/Textures/T_Default_C.dds");
	m_Normal = Texture2D::Create("Assets/Textures/T_Default_N.dds");
	m_MaterialInfo = Texture2D::Create("Assets/Textures/T_Default_M.dds");
	m_Registry[name] = std::make_shared<Material>(*this);
	m_ShaderKey = "Forward";
}

void Engine::Material::SetShader(const std::string& shaderKey)
{
	m_ShaderKey = shaderKey;
}

void Engine::Material::AddDiffuseTexture(Ref<Texture2D> texture)
{
	m_Diffuse = std::move(texture);
}

void Engine::Material::AddNormalTexture(Ref<Texture2D> texture)
{
	m_Normal = std::move(texture);
}

void Engine::Material::AddSpecularTexture(Ref<Texture2D> texture)
{
	m_MaterialInfo = std::move(texture);
}

void Engine::Material::Bind()
{
	ShaderLibrary::Bind(m_ShaderKey);
	m_Diffuse->Bind(0, ShaderType::Pixel);
	m_Normal->Bind(1, ShaderType::Pixel);
	m_MaterialInfo->Bind(2, ShaderType::Pixel);
}

Engine::Ref<Engine::Material> Engine::Material::Create(const std::string& name)
{
	if (m_Registry.contains(name))
	{
		return m_Registry[name];
	}
	return CreateRef<Material>(name);
}
