#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Engine/Renderer/Renderer.h"
namespace Engine
{
	void Scene::OnEditorStart()
	{
	}

	void Scene::OnEditorUpdate()
	{

		m_Registry.Execute<ModelComponent, TransformComponent>([&](auto& entity, ModelComponent& mdl, TransformComponent& tf)
			{
				if (!mdl.modelHandle)
				{
					if (std::filesystem::exists(mdl.filePath))
						mdl.modelHandle = Model::Create(mdl.filePath);
				}
				mdl.modelHandle->GetTransform().SetPosition(tf.transform.GetPosition());
				mdl.modelHandle->GetTransform().SetRotation(tf.transform.GetRotation());
				mdl.modelHandle->GetTransform().SetScale(tf.transform.GetScale());
				Renderer::SubmitMesh(mdl.modelHandle.get());
			});
	}

	void Scene::OnEditorRender()
	{
	}

	Entity Scene::CreateEntity()
	{
		Entity ent = { m_Registry.CreateEntity(), this };
		ent.AddComponent<TagComponent>();
		ent.AddComponent<TransformComponent>();
		return ent;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.DestroyEntity(entity.m_EntityId);
	}
}