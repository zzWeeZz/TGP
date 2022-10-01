#include "SceneSerializer.h"
#include "Entity.h"
#include "yaml-cpp/yaml.h"
#include "Components.h"
#include "Engine/Scripting/ScriptRegistry.h"
#include "Engine/Assets/AssetPacker.h"

Engine::SceneSerializer::SceneSerializer(Ref<Scene> scene)
{
    myScene = scene;
}

YAML::Emitter& operator<<(YAML::Emitter& out, Vector3f& vector)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
	return out;
}


void Engine::SceneSerializer::SerializeEntity(Engine::Entity entity, YAML::Emitter& out)
{
	out << YAML::BeginMap;
	out << YAML::Key << "Entity" << YAML::Value << entity.GetId();

	if (entity.HasComponent<TagComponent>())
	{
		out << YAML::Key << "TagComponent";
		out << YAML::Value << YAML::BeginMap;

		auto& tag = entity.GetComponent<TagComponent>();
		out << YAML::Key << "Tag" << YAML::Value << tag.tag.c_str();

		out << YAML::EndMap;
	}

	if (entity.HasComponent<TransformComponent>())
	{
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap;

		auto& transform = entity.GetComponent<TransformComponent>();
		out << YAML::Key << "Position" << YAML::Value << transform.transform.GetPosition();
		out << YAML::Key << "Rotation" << YAML::Value << transform.transform.GetRotation();
		out << YAML::Key << "Scale" << YAML::Value << transform.transform.GetScale();

		out << YAML::EndMap;
	}

	if (entity.HasComponent<ModelComponent>())
	{
		out << YAML::Key << "ModelComponent";
		out << YAML::BeginMap;

		auto& mesh = entity.GetComponent<ModelComponent>();
		out << YAML::Key << "Mesh" << YAML::Value << mesh.filePath.c_str();

		out << YAML::EndMap;
	}

	if (entity.HasComponent<AnimatorComponent>())
	{
		out << YAML::Key << "AnimatorComponent";
		out << YAML::BeginMap;

		auto& mesh = entity.GetComponent<AnimatorComponent>();
		out << YAML::Key << "Animator" << YAML::Value << mesh.animatorPath.c_str();

		out << YAML::EndMap;
	}

	if (entity.HasComponent<PointLightComponent>())
	{
		out << YAML::Key << "PointLightComponent";
		out << YAML::BeginMap;

		auto& light = entity.GetComponent<PointLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << light.color;
		out << YAML::Key << "Intensity" << YAML::Value << light.intensity;
		out << YAML::Key << "Range" << YAML::Value << light.radius;

		out << YAML::EndMap;
	}

	if (entity.HasComponent<DirectionalLightComponent>())
	{
		out << YAML::Key << "DirLightComponent";
		out << YAML::BeginMap;

		auto& light = entity.GetComponent<DirectionalLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << light.color;
		out << YAML::Key << "Intensity" << YAML::Value << light.intensity;

		out << YAML::EndMap;
	}

	if (entity.HasComponent<SpotLightComponent>())
	{
		out << YAML::Key << "SpotLightComponent";
		out << YAML::BeginMap;

		auto& light = entity.GetComponent<SpotLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << light.color;
		out << YAML::Key << "Intensity" << YAML::Value << light.intensity;
		out << YAML::Key << "Range" << YAML::Value << light.cutoff;
		out << YAML::Key << "near" << YAML::Value << light.nearRadius;
		out << YAML::Key << "far" << YAML::Value << light.farRadius;

		out << YAML::EndMap;
	}

	if (entity.HasComponent<ScriptComponent>())
	{
		out << YAML::Key << "ScriptComponent";
		out << YAML::BeginMap;

		auto& sc = entity.GetComponent<ScriptComponent>();
		out << YAML::Key << "Scripts" << YAML::Value << YAML::BeginSeq;
		for (auto& script : sc.scripts)
		{
			out << script->GetName();
		}
		out << YAML::EndSeq;

		out << YAML::EndMap;
	}
	out << YAML::EndMap;
}

void Engine::SceneSerializer::DeserializeEntity(Engine::Entity& entity, YAML::Node& node)
{
	std::string name;
	auto tagComponent = node["TagComponent"];
	if (tagComponent)
	{
		name = tagComponent["Tag"].as<std::string>();
	}

	;
	Entity& DeserializedEntity = entity;
	DeserializedEntity.GetComponent<TagComponent>().tag = name;
	auto transformComponent = node["TransformComponent"];
	if (transformComponent)
	{
		auto& tf = DeserializedEntity.GetComponent<TransformComponent>();
		Vector3f vec3;
		auto pos = transformComponent["Position"].as<std::vector<float>>();
		auto rot = transformComponent["Rotation"].as<std::vector<float>>();
		auto scl = transformComponent["Scale"].as<std::vector<float>>();
		memcpy(&vec3, pos.data(), sizeof(Vector3f));
		tf.transform.SetPosition(vec3);
		memcpy(&vec3, rot.data(), sizeof(Vector3f));
		tf.transform.SetRotation(vec3);
		memcpy(&vec3, scl.data(), sizeof(Vector3f));
		tf.transform.SetScale(vec3);
	}

	auto meshComponent = node["ModelComponent"];
	if (meshComponent)
	{
		auto& mc = DeserializedEntity.AddComponent<ModelComponent>();
		auto str = meshComponent["Mesh"].as<std::string>();
		mc.filePath = str.c_str();
	}

	auto animatorComponent = node["AnimatorComponent"];
	if (animatorComponent)
	{
		auto& mc = DeserializedEntity.AddComponent<AnimatorComponent>();
		auto str = animatorComponent["Animator"].as<std::string>();
		mc.animatorPath = str.c_str();
		AssetPacker::ReadAnimator(mc.animatorPath, mc.skPath, mc.specs);
		mc.modelHandle = AnimatedModel::Create(mc.skPath);
		for (size_t i = 0; i < mc.specs.size(); ++i)
		{
			mc.modelHandle->AddAnimation(mc.specs[i]);
		}
		mc.modelHandle->PlayAnimation(mc.specs[0].Name);
	}

	auto pointlightComponent = node["PointLightComponent"];
	if (pointlightComponent)
	{
		auto& pc = DeserializedEntity.AddComponent<PointLightComponent>();
		memcpy(&pc.color, pointlightComponent["Color"].as<std::vector<float>>().data(), sizeof(Vector3f));
		pc.intensity = pointlightComponent["Intensity"].as<float>();
		pc.radius = pointlightComponent["Range"].as<float>();
	}

	auto dirlightComponent = node["DirLightComponent"];
	if (dirlightComponent)
	{
		auto& pc = DeserializedEntity.AddComponent<DirectionalLightComponent>();
		memcpy(&pc.color, dirlightComponent["Color"].as<std::vector<float>>().data(), sizeof(Vector3f));
		pc.intensity = dirlightComponent["Intensity"].as<float>();
	}

	auto spotlightComponent = node["SpotLightComponent"];
	if (spotlightComponent)
	{
		auto& pc = DeserializedEntity.AddComponent<SpotLightComponent>();
		memcpy(&pc.color, spotlightComponent["Color"].as<std::vector<float>>().data(), sizeof(Vector3f));
		pc.intensity = spotlightComponent["Intensity"].as<float>();
		pc.cutoff = spotlightComponent["Range"].as<float>();
		pc.nearRadius = spotlightComponent["near"].as<float>();
		pc.farRadius = spotlightComponent["far"].as<float>();
	}

	auto Script = node["ScriptComponent"];
	if (Script)
	{
		auto& dlc = DeserializedEntity.AddComponent<ScriptComponent>();
		auto scriptNames = Script["Scripts"].as<std::vector<std::string>>();
		for (auto& sn : scriptNames)
		{
			auto s = ScriptRegistry<ScriptBase>::Create(sn.c_str());
			s->Create(DeserializedEntity);
			dlc.scripts.push_back(s);
		}
	}
}

bool Engine::SceneSerializer::Serialize(const std::string& path)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Name";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	myScene->m_Registry.ForEach([&](auto& e)
		{

			Entity entity = { e, myScene.get() };
			if (entity())
			{
				SerializeEntity(entity, out);
			}

		});
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(path);
	fout << out.c_str();
	return true;
}

bool Engine::SceneSerializer::Deserialize(const std::string& path)
{
	std::ifstream fin(path);
	std::stringstream buffer;
	buffer << fin.rdbuf();
	YAML::Node node = YAML::Load(buffer.str());
	if (!node["Scene"])
	{
		return false;
	}
	std::string sceneName = node["Scene"].as<std::string>();

	auto entities = node["Entities"].as<std::vector<YAML::Node>>();
	if (!entities.empty())
	{
		for (auto entityData : entities)
		{
			//uint64_t entityID = entityData["Entity"].as<uint64_t>();
			Entity entity = myScene->CreateEntity();
			DeserializeEntity(entity, entityData);
		}
	}
	return true;
}

