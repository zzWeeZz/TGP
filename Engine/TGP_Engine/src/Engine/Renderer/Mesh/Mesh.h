#pragma once
#include <future>
#include <string>

#include "SubMesh.h"
#include "Engine/DX11/DX11.h"

namespace TGA
{
	struct FBXModel;
}
	
namespace Engine
{
	class Mesh
	{
	public:
		void SetMesh(const std::filesystem::path& aPath, std::vector<Ref<Material>>& outMaterials);
		void SubmitMesh();

	private:
		void LoadMesh(const std::filesystem::path& aPath, std::vector<Ref<Material>>& outMaterials);
		void ProcessMaterials(const std::filesystem::path& path, TGA::FBXModel& FBXmodel, std::vector<Ref<Material>>& outMaterials);
		std::vector<SubMesh> m_SubMeshes;
	};
}
