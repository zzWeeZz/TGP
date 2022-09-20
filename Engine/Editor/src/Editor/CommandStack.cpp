#include "CommandStack.h"
#include "ToolBox/Input/Input.h"
namespace Engine
{
	void CommandStack::AddCallback(std::function<void(CommandSet)>&& func)
	{
		s_FuncList.push_back(func);
	}
	void CommandStack::Update()
	{
		if (!s_Stack.empty())
		{
			if (ToolBox::Input::IsKeyDown(Keys::Control) && ToolBox::Input::IsKeyPressed(Keys::Z))
			{
				auto undoCommand = s_Stack.top();
				for (auto& func : s_FuncList)
				{
					func(undoCommand);
				}
				s_Stack.pop();
			}
		}
	}
}