#include "InputHandler.h"
namespace ToolBox
{
	bool InputHandler::IsKeyPressed(const int aKeyCode) const
	{
		assert(aKeyCode > 6 && L"Keycode does not refer to keyboard key");
		if (myPastKeyPresses.test(aKeyCode) == false && myCurrentKeyPresses.test(aKeyCode))
		{
			return true;
		}
		return false;
	}

	bool InputHandler::IsKeyHeld(const int aKeyCode) const
	{
		assert(aKeyCode > 6 && L"Keycode does not refer to keyboard key");
		if (myPastKeyPresses.test(aKeyCode) && myCurrentKeyPresses.test(aKeyCode))
		{
			return true;
		}
		return false;
	}

	bool InputHandler::IsKeyReleased(const int aKeyCode) const
	{
		assert(aKeyCode > 6 && L"Keycode does not refer to keyboard key");
		if (myPastKeyPresses.test(aKeyCode) && myCurrentKeyPresses.test(aKeyCode) == false)
		{
			return true;
		}
		return false;
	}

	POINT InputHandler::GetMousePosition() const
	{
		return myCurrentMousePos;
	}

	POINT InputHandler::GetMouseMovement() const
	{
		int x = (myPastMousePos.x - myCurrentMousePos.x);
		int y = (myPastMousePos.y - myCurrentMousePos.y);
		POINT point{ x,y };
		return point;
	}

	int InputHandler::GetScrollWheelDelta() const
	{
		return (myCurrentScrollWheelDelta - myPastScrollWheelDelta);
	}

	bool InputHandler::IsMouseKeyPressed(const int aKeyCode) const
	{
		assert(aKeyCode <= 6 && L"Keycode does not refer to mouse key");
		assert(aKeyCode != 3 && L"Keycode does not refer to mouse key");

		if (myPastKeyPresses.test(aKeyCode) == false && myCurrentKeyPresses.test(aKeyCode))
		{
			return true;
		}
		return false;
	}

	bool InputHandler::IsMouseKeyHeld(const int aKeyCode) const
	{
		assert(aKeyCode <= 6 && L"Keycode does not refer to mouse key");
		assert(aKeyCode != 3 && L"Keycode does not refer to mouse key");
		if (myPastKeyPresses.test(aKeyCode) && myCurrentKeyPresses.test(aKeyCode))
		{
			return true;
		}
		return false;
	}

	bool InputHandler::IsMouseKeyReleased(const int aKeyCode) const
	{
		assert(aKeyCode <= 6 && L"Keycode does not refer to mouse key");
		assert(aKeyCode != 3 && L"Keycode does not refer to mouse key");
		if (myPastKeyPresses.test(aKeyCode) && myCurrentKeyPresses.test(aKeyCode) == false)
		{
			return true;
		}
		return false;
	}

	bool InputHandler::UpdateEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam)
	{
		switch (aMessage)
		{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				myTentativeKeyPresses.set(aWParam);
				return true;
				break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				myTentativeKeyPresses.reset(aWParam);
				break;

			default:
				break;
		}

		switch (aMessage)
		{
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
				myTentativeKeyPresses.set(aWParam);
				return true;
				break;

			case WM_MBUTTONDOWN:
				myTentativeKeyPresses.set(0x04);
				return true;
				break;

			case WM_XBUTTONDOWN:
				if (aWParam == 0x0020)
				{
					myTentativeKeyPresses.set(0x05);
				}
				else if (aWParam == 0x0040)
				{
					myTentativeKeyPresses.set(0x06);
				}
				return true;
				break;

			case WM_LBUTTONUP:
				myTentativeKeyPresses.reset(0x01);
				break;

			case WM_RBUTTONUP:
				myTentativeKeyPresses.reset(0x02);
				break;

			case WM_MBUTTONUP:
				myTentativeKeyPresses.reset(0x04);
				break;

			case WM_XBUTTONUP:
				myTentativeKeyPresses.reset(0x05);
				myTentativeKeyPresses.reset(0x06);
				break;

			default:
				break;
		}

		switch (aMessage)
		{
			case WM_MOUSEMOVE:
				POINT point;
				GetCursorPos(&point);
				ScreenToClient(GetActiveWindow(), &point);
				myTentativeMousePos = point;
				return true;
				break;

			case WM_MOUSEWHEEL:
				myTentativeScrollWheelDelta += (GET_WHEEL_DELTA_WPARAM(aWParam) / 120);
				//Safeguard to avoid integer overflow/underflow for scroll wheel delta
				if (myTentativeScrollWheelDelta > 10000)
				{
					myTentativeScrollWheelDelta -= 10000;
					myPastScrollWheelDelta -= 10000;
				}
				else if (myTentativeScrollWheelDelta < -10000)
				{
					myTentativeScrollWheelDelta += 10000;
					myPastScrollWheelDelta += 10000;
				}
				return true;
				break;

			default:
				break;
		}
		return false;
	}

	void InputHandler::SetMousePosition(POINT& aMousePos) const
	{
		ClientToScreen(GetActiveWindow(), &aMousePos);
		SetCursorPos(aMousePos.x, aMousePos.y);
	}

	void InputHandler::CaptureMouse(bool aAllowWindowInteraction) const
	{
		if (aAllowWindowInteraction == false)
		{
			SetCapture(GetActiveWindow());
		}
		RECT rect;
		GetWindowRect(GetActiveWindow(), &rect);
		ClipCursor(&rect);
	}

	void InputHandler::ReleaseMouse() const
	{
		ReleaseCapture();
		ClipCursor(NULL);
	}

	void InputHandler::Update()
	{
		myPastKeyPresses = myCurrentKeyPresses;
		myCurrentKeyPresses = myTentativeKeyPresses;

		myPastMousePos = myCurrentMousePos;
		myCurrentMousePos = myTentativeMousePos;

		myPastScrollWheelDelta = myCurrentScrollWheelDelta;
		myCurrentScrollWheelDelta = myTentativeScrollWheelDelta;
    }
}

