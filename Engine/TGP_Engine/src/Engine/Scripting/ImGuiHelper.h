#pragma once
#include <vector>

namespace Engine
{
	enum class PropertyType
	{
		Float,
		Vector2,
		Vector3,
		Vector4,
		String,
		Int,
		Count
	};
	struct Property
	{
		PropertyType type = PropertyType::Count;
		std::string name = "varible";
		void* varible = nullptr;
		Property(PropertyType a, const std::string& n, void* var)
		{
			type = a;
			name = n;
			varible = var;
		}
	};
	struct Properties
	{
		Properties() = default;
		Properties(const std::initializer_list<Property>& props)
		{
			properties = props;
		}
		std::vector<Property> properties = {};
	};
}