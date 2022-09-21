#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Engine/Renderer/Renderer.h"
#include "ToolBox/Math/Matrix/Matrix.h"
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
					if (std::filesystem::exists(mdl.filePath.c_str()))
						mdl.modelHandle = Model::Create(mdl.filePath.c_str());
				}
				else
				{

					mdl.modelHandle->GetTransform().SetPosition(tf.transform.GetPosition());
					mdl.modelHandle->GetTransform().SetRotation(tf.transform.GetRotation());
					mdl.modelHandle->GetTransform().SetScale(tf.transform.GetScale());
					mdl.modelHandle->SetEntity(entity);
					Renderer::SubmitMesh(mdl.modelHandle.get());
				}
			});

		m_Registry.Execute<AnimatorComponent, TransformComponent>([&](auto& entity, AnimatorComponent& mdl, TransformComponent& tf)
			{
				if (!mdl.modelHandle)
				{
					if (std::filesystem::exists(mdl.skPath.c_str()))
					{
						mdl.modelHandle = AnimatedModel::Create(mdl.skPath.c_str());
						for (size_t i = 0; i < mdl.specs.size(); ++i)
						{
							mdl.modelHandle->AddAnimation(mdl.specs[i]);
						}
					}
				}
				else
				{
					mdl.modelHandle->GetTransform().SetPosition(tf.transform.GetPosition());
					mdl.modelHandle->GetTransform().SetRotation(tf.transform.GetRotation());
					mdl.modelHandle->GetTransform().SetScale(tf.transform.GetScale());
					mdl.modelHandle->RunAnimation();
					Renderer::SubmitAnimatedMesh(mdl.modelHandle.get());
				}
			});
		m_Registry.Execute<PointLightComponent, TransformComponent>([&](auto& entity, PointLightComponent& pl, TransformComponent& tf)
			{
				PointLightData plData;
				plData.colorAndIntensity = { pl.color.x, pl.color.y , pl.color.z, pl.intensity };
				plData.position = tf.transform.GetPosition();
				plData.radius = pl.radius;
				Matrix4x4f shadowProj = Matrix4x4f::CreateLeftHandPerspectiveMatrix(90, { 1,1 }, 0.001f, plData.radius * 2);

				std::array<Matrix4x4f, 6> lookats;
				lookats[4] = Matrix4x4f::CreateLookAt(plData.position, plData.position + Vector3f(0, 0, -1.0), Vector3f(0, 1, 0));
				lookats[5] = Matrix4x4f::CreateLookAt(plData.position, plData.position + Vector3f(0, 0, 1.0), Vector3f(0, 1, 0));
				lookats[2] = Matrix4x4f::CreateLookAt(plData.position, plData.position + Vector3f(0, -1.0, 0), Vector3f(0, 0, 1));
				lookats[3] = Matrix4x4f::CreateLookAt(plData.position, plData.position + Vector3f(0, 1.0, 0), Vector3f(0, 0, -1));
				lookats[0] = Matrix4x4f::CreateLookAt(plData.position, plData.position + Vector3f( -1.0, 0, 0), Vector3f(0, 1, 0));
				lookats[1] = Matrix4x4f::CreateLookAt(plData.position, plData.position + Vector3f(1.0, 0, 0), Vector3f(0, 1, 0));

				for (uint32_t i = 0; i < 6; ++i)
				{
					plData.transforms[i] = lookats[i] * shadowProj;
				}
			

				Renderer::SubmitPointLight(plData);
			});

		m_Registry.Execute<SpotLightComponent, TransformComponent>([&](auto& entity, SpotLightComponent& pl, TransformComponent& tf)
			{
				SpotLightData plData;
				plData.colorAndIntensity = { pl.color.x, pl.color.y , pl.color.z, pl.intensity };
				plData.position = { tf.transform.GetPosition().x,tf.transform.GetPosition().y, tf.transform.GetPosition().z, 0 };
				plData.direction = { tf.transform.GetForward().x,tf.transform.GetForward().y, tf.transform.GetForward().z, 0 };
				plData.spotInfo = { pl.cutoff, pl.nearRadius, pl.farRadius, 0 };
				Renderer::SubmitSpotLight(plData);
			});

		m_Registry.Execute<DirectionalLightComponent, TransformComponent>([&](auto& entity, DirectionalLightComponent& pl, TransformComponent& tf)
			{
				DirLightData plData;
				plData.colorAndIntensity = { pl.color.x, pl.color.y , pl.color.z, pl.intensity };
				plData.direction = { tf.transform.GetForward().x,tf.transform.GetForward().y, tf.transform.GetForward().z, 0 };
				Matrix4x4f view;
				memcpy(&view, &tf.transform.GetMatrix(), sizeof(Matrix4x4f));
				plData.view = Matrix4x4f::CreateLookAt(tf.transform.GetForward() * 10000.f, { 0,0,0 }, { 0,1,0 });
				plData.proj = Matrix4x4f::CreateOrtographicMatrix({ 2500, 2500 }, 1.f, 12000.0);
				Renderer::SubmitDirectionalLight(plData);
			});

		m_Registry.Execute<ScriptComponent>([&](auto&, ScriptComponent& sc)
			{
				for (auto& script : sc.scripts)
				{
					script->Update();
				}
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