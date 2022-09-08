#include "AssetPacker.h"
#include "yaml-cpp/yaml.h"
#include "Engine/Scene/SceneSerializer.h"
#include <fstream>
YAML::Emitter& operator<<(YAML::Emitter& out, Vector4f& vector)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vector.x << vector.y << vector.z << vector.w << YAML::EndSeq;
	return out;
}

void Engine::AssetPacker::Particle(Ref<ParticleSystem> ps, const std::string& name)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "ParticleSystem" << YAML::Value << name;
	out << YAML::Key << "Emitters" << YAML::Value << YAML::BeginSeq;
	for (auto& emit : ps->GetEmitters())
	{
		out << YAML::Key << "StartColor" << YAML::Value << emit.myData.startColor;
		out << YAML::Key << "EndColor" << YAML::Value << emit.myData.endColor;
		out << YAML::Key << "StartSize" << YAML::Value << emit.myData.startSize;
		out << YAML::Key << "EndSize" << YAML::Value << emit.myData.endSize;
		
		out << YAML::Key << "StartVelocity" << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << emit.myData.startVelocity.x << emit.myData.startVelocity.y << emit.myData.startVelocity.z << YAML::EndSeq;
		out << YAML::Key << "EndVelocity" << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << emit.myData.endVelocity.x << emit.myData.endVelocity.y << emit.myData.endVelocity.z << YAML::EndSeq;
		out << YAML::Key << "LifeTime" << YAML::Value << emit.myData.lifeTime;
		out << YAML::Key << "LifeTimeVariance" << YAML::Value << emit.myData.lifeTimeVariance;
		out << YAML::Key << "SpwanRate" << YAML::Value << emit.myData.spawnRate;
		out << YAML::Key << "SpwanRateVariance" << YAML::Value << emit.myData.spawnRateVariance;
		out << YAML::Key << "MaxParticles" << YAML::Value << emit.myData.maxParticles;
		out << YAML::Key << "GravityScale" << YAML::Value << emit.myData.gravityScale;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	std::string path = "Assets/PatricleSystems/" + name + ".pts";
	std::ofstream fout(path);
	fout << out.c_str();
}

void Engine::AssetPacker::ReadAnimator(const std::string& path, std::string& skPath, std::vector<AnimationSpecs>& outSpecs)
{
	std::ifstream fin(path);
	std::stringstream buffer;
	buffer << fin.rdbuf();
	YAML::Node node = YAML::Load(buffer.str());
	skPath = node["SkPath"].as<std::string>();
	std::vector<YAML::Node> specs = node["AnimSpecs"].as<std::vector<YAML::Node>>();
	for (auto& spec : specs)
	{
		AnimationSpecs& s = outSpecs.emplace_back();
		s.Name = spec["Name"].as<std::string>();
		s.FilePath = spec["skPath"].as<std::string>();
		s.Interpolate = spec["Interp"].as<bool>();
		s.Loop = spec["Loop"].as<bool>();
		s.Speed = spec["Speed"].as<float>();
	}
}

void Engine::AssetPacker::WriteAnimator(const std::string& path, const std::string& skPath, std::vector<AnimationSpecs>& writeData)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "SkPath" << YAML::Value << skPath;
	out << YAML::Key << "AnimSpecs" << YAML::Value << YAML::BeginSeq;
	for (auto& spec : writeData)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << spec.Name;
		out << YAML::Key << "skPath" << YAML::Value << spec.FilePath.string();
		out << YAML::Key << "Interp" << YAML::Value << spec.Interpolate;
		out << YAML::Key << "Loop" << YAML::Value << spec.Loop;
		out << YAML::Key << "Speed" << YAML::Value << spec.Speed;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	std::ofstream fout(path);
	fout << out.c_str();
}
