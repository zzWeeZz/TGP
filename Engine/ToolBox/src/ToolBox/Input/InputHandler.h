#pragma once
#include <Windows.h>
#include <bitset>
#include <cassert>
namespace ToolBox
{
	class InputHandler
	{
		public:
		InputHandler() = default;
		~InputHandler() = default;


		bool IsKeyPressed(const int aKeyCode) const;
		bool IsKeyHeld(const int aKeyCode) const;
		bool IsKeyReleased(const int aKeyCode) const;

		bool UpdateEvents(UINT aMessage, WPARAM aWParam, LPARAM aLParam);

		bool IsMouseKeyPressed(const int aKeyCode) const;
		bool IsMouseKeyHeld(const int aKeyCode) const;
		bool IsMouseKeyReleased(const int aKeyCode) const;
		POINT GetMousePosition() const;
		POINT GetMouseMovement() const;
		int GetScrollWheelDelta() const;
		void SetMousePosition(POINT& aMousePos) const;
		void CaptureMouse(bool aAllowWindowInteraction) const;
		void ReleaseMouse() const;
		void Update();

		private:
		std::bitset<256> myTentativeKeyPresses;
		std::bitset<256> myCurrentKeyPresses;
		std::bitset<256> myPastKeyPresses;

		int myTentativeScrollWheelDelta;
		int myCurrentScrollWheelDelta;
		int myPastScrollWheelDelta;

		POINT myTentativeMousePos;
		POINT myCurrentMousePos;
		POINT myPastMousePos;
	};
}