#pragma once
#include "Shader.h"
#include "Engine/Utils/Utilities.h"
#include <unordered_map>

namespace Engine
{
	class ShaderLibrary
	{
	public:
		static void AddToLibrary(const std::string& name, const std::vector<Ref<Shader>>& shaders);
		static void Bind(const std::string& name);
		static void UnBind(const std::string& name);
		
	private:
		static std::unordered_map<std::string, std::vector<Ref<Shader>>> m_Library;
	};
}
