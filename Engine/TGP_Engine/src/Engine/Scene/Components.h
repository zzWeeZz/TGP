#pragma once
#include "SnowFlake/Serializer.hpp"
#include "SnowFlake/Snowflake.hpp"
#include "ToolBox/Math/Transform.h"
#include "Engine/Assets/Model/Model.h"
#include <Engine/Scripting/ScriptBase.h>
#include "SnowFlake/String.hpp"
#include "Engine/Renderer/Mesh/AnimatedMesh.h"
#include "Engine/Assets/Model/AnimatedModel.h"

COMPONENT(TagComponent)
{
	REGISTER_COMPONENT("{0A07B7F9-8312-40B1-81AA-773BE3B295B9}"_guid);
	std::string tag = "Unknown";
};

COMPONENT(TransformComponent)
{
	REGISTER_COMPONENT("{7B78818B-FBCF-4359-B016-8C8DF1D7F642}"_guid);
	ToolBox::Transform transform;
};

COMPONENT(ModelComponent)
{
	REGISTER_COMPONENT("{EC4E0F57-123F-4EEF-BF9E-DA7A05E0052A}"_guid);
	Engine::Ref<Engine::Model> modelHandle = nullptr;
	std::string filePath = "";
};

COMPONENT(PointLightComponent)
{
	REGISTER_COMPONENT("{007E3992-3BA8-4798-A8BD-9CB2BE2096D3}"_guid);
	Vector3f color;
	float intensity = 0;
	float radius = 0;
};

COMPONENT(SpotLightComponent)
{
	REGISTER_COMPONENT("{9DEF3C99-74FB-457E-8376-65701E9BBF15}"_guid);
	Vector3f color;
	float intensity = 0;
	float cutoff = 0;
	float nearRadius = 0;
	float farRadius = 0;
};

COMPONENT(DirectionalLightComponent)
{
	REGISTER_COMPONENT("{1C956EE5-3222-4172-A088-836BADD13CB8}"_guid);
	Vector3f color;
	float intensity = 0;
};

COMPONENT(AnimatorComponent)
{
	REGISTER_COMPONENT("{481D7E20-A5F6-4AA5-8C35-AF3105265E88}"_guid);
	std::string animatorPath = "";
	std::string skPath = "";
	std::vector<Engine::AnimationSpecs> specs;
	Engine::Ref<Engine::AnimatedModel> modelHandle;
};

COMPONENT(ScriptComponent)
{
	REGISTER_COMPONENT("{9940D790-3E67-4C0B-A1B8-FCE018E3BC70}"_guid);
	std::vector<Engine::ScriptBase*> scripts;
};