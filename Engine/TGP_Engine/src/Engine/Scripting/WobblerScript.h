#pragma once
#include "ScriptBase.h"
namespace Engine
{
	class Wobbler : public ScriptBase
	{
	public:
		virtual Properties SetProperties() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnDestroy() override;
		virtual std::string GetName() override { return "Wobbler"; }
		static ScriptBase* Create()
		{
			return new Wobbler();
		}
	protected:
	private:

		float myWobbleDamage;
		
	};
}