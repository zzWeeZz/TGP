#include "Input.h"

ToolBox::InputHandler ToolBox::Input::myInputHandler;
Vector2i ToolBox::Input::myMouseDelta;
Vector2i ToolBox::Input::myOldMousePos;
bool ToolBox::Input::IsKeyDown(Keys key)
{
	return myInputHandler.IsKeyHeld((int)key);
}

bool ToolBox::Input::IsKeyPressed(Keys key)
{
	return myInputHandler.IsKeyPressed((int)key);
}

bool ToolBox::Input::IsKeyUp(Keys key)
{
	return myInputHandler.IsKeyReleased((int)key);
}

Vector2i ToolBox::Input::GetMousePosition()
{
	return {myInputHandler.GetMousePosition().x, myInputHandler.GetMousePosition().y};
}

Vector2i ToolBox::Input::GetMouseDelta()
{
	return myMouseDelta;
}

bool ToolBox::Input::IsMouseDown(MouseButton button)
{
	return myInputHandler.IsMouseKeyHeld((int)button);
}

bool ToolBox::Input::IsMousePressed(MouseButton button)
{
	return myInputHandler.IsMouseKeyPressed((int)button);
}

bool ToolBox::Input::IsMouseUp(MouseButton button)
{
	return myInputHandler.IsMouseKeyReleased((int)button);
}

int ToolBox::Input::GetMouseWheelDelta()
{
	return myInputHandler.GetScrollWheelDelta();
}

void ToolBox::Input::UpdateData()
{
	myInputHandler.Update();
	// calc mouse delta
	myMouseDelta = GetMousePosition() - myOldMousePos;
	myOldMousePos = GetMousePosition();
	
}

bool ToolBox::Input::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	return myInputHandler.UpdateEvents(message, wParam, lParam);
}
