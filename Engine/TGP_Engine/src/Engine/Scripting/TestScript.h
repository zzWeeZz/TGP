#pragma once
#include "ScriptBase.h"

namespace Engine
{
	class TestScript : public ScriptBase
	{
	public:
		// Inherited via ScriptBase
		virtual Properties SetProperties() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnDestroy() override;
	private:
		float mySpeed;
		int myInt;
		float myAnotherFloat;
	};
}