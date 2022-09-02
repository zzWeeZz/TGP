#pragma once

#include <functional>
#define REGISTER_SCRIPT(scriptName, createFunc) \
	bool scriptName ## _entity = ScriptRegistry<ScriptBase>::Add(#scriptName, (createFunc))
namespace Engine
{
	template<typename T>
	class ScriptRegistry
	{
	public:
		typedef std::function<T* ()> FactoryFunc;
		typedef std::unordered_map<std::string, FactoryFunc> FactoryMap;

		static bool Add(const std::string& name, FactoryFunc func)
		{
			auto& map = GetFactoryMap();
			if (map.find(name) != map.end())
			{
				return false;
			}
			map[name] = func;
			return true;
		}
		static T* Create(const std::string& name)
		{
			auto& map = GetFactoryMap();
			if (map.find(name) == map.end())
			{
				return nullptr;
			}
			return map[name]();
		}

		static std::vector<std::string> GetNameList()
		{
			std::vector<std::string> returnValue;
			auto map = GetFactoryMap();
			for (auto& it : map)
			{
				returnValue.push_back(it.first);
			}
			return returnValue;
		}
	private:
		static FactoryMap& GetFactoryMap()
		{
			static FactoryMap map;
			return map;
		}
	};
}