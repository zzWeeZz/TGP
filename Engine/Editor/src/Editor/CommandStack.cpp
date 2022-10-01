#include "CommandStack.h"
#include "ToolBox/Input/Input.h"
namespace Engine
{
	void CommandStack::AddCallback(std::function<void(CommandSet)>&& func)
	{
		s_FuncList.push_back(func);
	}
	void CommandStack::WipeReDoCache()
	{
		while (!s_ReDoStack.empty())
		{
			s_ReDoStack.pop();
		}
	}
	void CommandStack::Update()
	{
		if (!s_Stack.empty())
		{
			if (ToolBox::Input::IsKeyDown(Keys::Control) && ToolBox::Input::IsKeyPressed(Keys::Z))
			{
				if (s_Stack.size() < 2)
				{
					while (!s_Stack.empty())
					{
						s_Stack.pop();
					}
					return;
				}
				auto redoCommand = s_Stack.top();
				s_ReDoStack.push(redoCommand);
				s_Stack.pop();
				auto undoCommand = s_Stack.top();
				for (auto& func : s_FuncList)
				{
					func(undoCommand);
				}
				s_ReDoStack.push(redoCommand);
				s_Stack.pop();
			}
		}
		if (!s_ReDoStack.empty())
		{
			if (ToolBox::Input::IsKeyDown(Keys::Control) && ToolBox::Input::IsKeyPressed(Keys::Y))
			{
				auto redoCommand = s_ReDoStack.top();
				s_Stack.push(redoCommand);
				s_ReDoStack.pop();
				auto undoCommand = s_ReDoStack.top();
				for (auto& func : s_FuncList)
				{
					func(undoCommand);
				}
				s_ReDoStack.pop();
			}
		}
	}
}