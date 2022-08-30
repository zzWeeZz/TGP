#pragma once
#include "SnowFlake/Serializer.hpp"
#include "SnowFlake/Snowflake.hpp"
#include "ToolBox/Math/Transform.h"

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
	std::string filePath;
	/*Titan::Ref<Titan::Model> modelHandle = nullptr;*/
};