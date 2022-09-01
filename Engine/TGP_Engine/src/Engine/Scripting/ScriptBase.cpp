#include "ScriptBase.h"

void Engine::ScriptBase::Create(Entity& entity)
{
	myEntity = entity;
	myProperties = SetProperties();
}
