#include "ShaderLibrary.h"
std::unordered_map<std::string, std::vector<Engine::Ref<Engine::Shader>>> Engine::ShaderLibrary::m_Library;

void Engine::ShaderLibrary::AddToLibrary(const std::string& name, const std::vector<Ref<Shader>>& shaders)
{
	m_Library[name] = shaders;
}

void Engine::ShaderLibrary::Bind(const std::string& name)
{
	if (m_Library.contains(name))
	{
		for (auto& shader : m_Library[name])
		{
			shader->Bind();
		}
	}
	else
	{
		std::cout << "INVALID SHADER!!!\n";
	}
}

void Engine::ShaderLibrary::UnBind(const std::string& name)
{
	if (m_Library.contains(name))
	{
		for (auto& shader : m_Library[name])
		{
			shader->UnBind();
		}
	}
	else
	{
		std::cout << "INVALID SHADER!!!\n";
	}
}
