#include "WobblerScript.h"
#include "ScriptRegistry.h"
#include "Engine/Scene/Components.h"
#include <ToolBox/Utility/Chrono.h>
namespace Engine
{
    REGISTER_SCRIPT(Wobbler, Wobbler::Create);
}

Engine::Properties Engine::Wobbler::SetProperties()
{
    Properties prop =
    {
        {PropertyType::Float, "WobblerSpeed", &myWobbleDamage}
    };
    return prop;
}

void Engine::Wobbler::Start()
{
}

void Engine::Wobbler::Update()
{
	static float angle;
	angle += myWobbleDamage * ToolBox::Chrono::Timestep();
	GetComponent<TransformComponent>().transform.SetPosition({ 0, cos(angle) * 100 ,0 });
}

void Engine::Wobbler::OnDestroy()
{
}
