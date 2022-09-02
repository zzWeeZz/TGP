#include "TestScript.h"
#include "Engine/Scene/Components.h"
#include <ToolBox/Utility/Chrono.h>
namespace Engine
{
	REGISTER_SCRIPT(TestScript, TestScript::Create);
}

Engine::Properties Engine::TestScript::SetProperties()
{
	Properties properties
	{
		{PropertyType::Float, "Speed", &mySpeed},
		{PropertyType::Int, "inter", &myInt},
		{PropertyType::Float, "hello 2", &myAnotherFloat},
	};
	return properties;
}

void Engine::TestScript::Start()
{
}

void Engine::TestScript::Update()
{
	static float angle;
	angle += mySpeed * ToolBox::Chrono::Timestep();
	GetComponent<TransformComponent>().transform.SetRotation({ 0, angle ,0 });
}

void Engine::TestScript::OnDestroy()
{
}
