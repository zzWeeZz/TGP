#pragma once
#include "ScriptBase.h"
#include "ScriptRegistry.h"
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

		virtual std::string GetName() { return "TestScript"; }
		
		static ScriptBase* Create()
		{
			return new TestScript();
		}
	private:
		float mySpeed;
		int myInt;
		float myAnotherFloat;
	};
}
