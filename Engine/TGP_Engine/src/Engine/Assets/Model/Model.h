#pragma once
#include "Engine/Assets/Asset.h"
#include "Engine/Utils/Utilities.h"
#include "Engine/Renderer/Mesh/Mesh.h"
#include "SnowFlake/Snowflake.hpp"
#include "ToolBox/Math/Transform.h"
namespace Engine
{
	class Model : public Asset
	{
	public:
		Model(const std::filesystem::path& filePath);
		std::vector<Ref<Material>>& GetMaterials() { return m_Materials; }
		void Draw();
		void SetEntity(Snowflake::Entity ett) {  m_Entity = ett; }
		uint32_t GetEntity() { return m_Entity; }
		ToolBox::Transform& GetTransform() { return myTransform; }
		
		static Ref<Model> Create(const std::filesystem::path& filePath);
	private:
		Snowflake::Entity m_Entity;
		std::vector<Ref<Material>> m_Materials;
		Mesh myMesh;
		ToolBox::Transform myTransform;
	};
}