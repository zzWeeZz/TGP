#pragma once
#include "Engine/Assets/Asset.h"
#include "Engine/Utils/Utilities.h"
#include "Engine/Renderer/Mesh/Mesh.h"

#include "ToolBox/Math/Transform.h"
namespace Engine
{
	class Model : public Asset
	{
	public:
		Model(const std::filesystem::path& filePath);
		std::vector<Ref<Material>>& GetMaterials() { return m_Materials; }
		void Draw();

		ToolBox::Transform& GetTransform() { return myTransform; }
		
		static Ref<Model> Create(const std::filesystem::path& filePath);
	private:
		std::vector<Ref<Material>> m_Materials;
		Mesh myMesh;
		ToolBox::Transform myTransform;
	};
}