#pragma once
#include <stack>
#include <functional>
namespace Engine
{
	enum class CommandType
	{
		Transform = 64,
	};
	typedef std::pair<CommandType, std::vector<uint8_t>> CommandSet;
	class CommandStack
	{
	public:
		template<CommandType T>
		static void Register(void* data, uint32_t id = 10000000);
		template<CommandType T>
		static void RegisterCurrent(void* data, uint32_t id = 10000000);
		static void AddCallback(std::function<void(CommandSet)>&& func);
		static void WipeReDoCache();
		static void Update();
	private:
		inline static std::vector<std::function<void(CommandSet)>> s_FuncList;
		inline static std::stack<CommandSet> s_Stack;
		inline static std::stack<CommandSet> s_ReDoStack;
	};
	template<CommandType T>
	inline void CommandStack::Register(void* data, uint32_t id)
	{
		auto& set = s_Stack.emplace();
		set.first = T;
		set.second.resize((int)T + sizeof(uint32_t));
		memcpy(set.second.data(), data, (int)T);
		*(uint32_t*)(set.second.data() + 16) = id;
		/*while (!s_ReDoStack.empty())
		{
			s_ReDoStack.pop();
		}*/
	}
	template<CommandType T>
	inline void CommandStack::RegisterCurrent(void* data, uint32_t id)
	{
		auto& set = s_Stack.emplace();
		set.first = T;
		set.second.resize((int)T + sizeof(uint32_t));
		memcpy(set.second.data(), data, (int)T);
		*(uint32_t*)(set.second.data() + 16) = id;
		
	}
}
