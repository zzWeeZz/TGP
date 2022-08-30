#include "Entity.h"

namespace Engine
{
	Entity::Entity(Snowflake::Entity entity, Scene* scene)
	{
		m_EntityId = entity;
		m_Scene = scene;
	}
}