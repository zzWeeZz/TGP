#include "Mesh.h"

#include <future>
#include <iostream>
#include "FBXImporter/FBXImporter.h"

void Engine::Mesh::SetMesh(const std::filesystem::path& aPath, std::vector<Ref<Material>>& outMaterials)
{
	auto& device = DX11::Device();
	auto& context = DX11::Context();
	if (aPath == "Cube")
	{
		outMaterials.emplace_back(Material::Create("Default"));
		Vertex v[] =
		{
			// Front Face
			Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0, 1),
			Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0, 1),
			Vertex(1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0, 1),
			Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0, 1),

			// Back Face
			Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1, 1),
			Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1, 1),
			Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1, 1),
			Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1, 1),

			// Top Face
			Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1, 1),
			Vertex(-1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1, 1),
			Vertex(1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1, 1),
			Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1, 1),

			// Bottom Face
			Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0, 1),
			Vertex(1.0f, -1.0f, -1.0f,1.0f, 1.0f, 0, 1),
			Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0, 1),
			Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0, 1),

			// Left Face
			Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.3f, 0, 1),
			Vertex(-1.0f,  1.0f,  1.0f, 1.0f, 0.3f, 0, 1),
			Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.3f, 0, 1),
			Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.3f, 0, 1),

			// Right Face
			Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0, 1),
			Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0, 1),
			Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0, 1),
			Vertex(1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0, 1),
		};
		// generate UV Coords
		for (int i = 0; i < 6; i++)
		{
			v[i * 4].UV[0] = { 0,0 };
			v[i * 4 + 1].UV[0] = { 1,0 };
			v[i * 4 + 2].UV[0] = { 1,1 };
			v[i * 4 + 3].UV[0] = { 0,1 };
		}

		/*{
			int index = 0;
			v[index].normal = { -1, 0, 0 };
			Vector3f c1 = v[index].normal.Cross(Vector3f(0, 0, 1));
			Vector3f c2 = v[index].normal.Cross(Vector3f(0, 1, 0));
			if (c1.Length() > c2.Length())
				v[index].tangent = c1;
			else
				v[index].tangent = c2;

			v[index].tangent.Normalize();
			v[index].biTangent = v[index].normal.Cross(v[index].tangent).GetNormalized();
		}
		{
			int index = 1;
			v[index].normal = {  };
			Vector3f c1 = v[index].normal.Cross(Vector3f(0, 0, 1));
			Vector3f c2 = v[index].normal.Cross(Vector3f(0, 1, 0));
			if (c1.Length() > c2.Length())
				v[index].tangent = c1;
			else
				v[index].tangent = c2;

			v[index].tangent.Normalize();
			v[index].biTangent = v[index].normal.Cross(v[index].tangent).GetNormalized();
		}*/

		DWORD index[] =
		{
			// Front Face
			0,  1,  2,
			0,  2,  3,

			// Back Face
			4,  5,  6,
			4,  6,  7,

			// Top Face
			8,  9, 10,
			8, 10, 11,

			// Bottom Face
			12, 13, 14,
			12, 14, 15,

			// Left Face
			16, 17, 18,
			16, 18, 19,

			// Right Face
			20, 21, 22,
			20, 22, 23
		};

		std::vector<Vertex> verts;
		for (size_t i = 0; i < ARRAYSIZE(v); i++)
		{
			verts.emplace_back(v[i]);
		}
		std::vector<DWORD> indexVector;
		for (size_t i = 0; i < ARRAYSIZE(index); i++)
		{
			indexVector.emplace_back(index[i]);
		}


		m_SubMeshes.emplace_back(verts, indexVector, outMaterials[0]);
	}
	else
	{
		LoadMesh(aPath, outMaterials);
	}
}

void Engine::Mesh::SubmitMesh()
{
	for (auto& subMesh : m_SubMeshes)
	{
		subMesh.Draw();
	}
}

void Engine::Mesh::ProcessMaterials(const std::filesystem::path& path, TGA::FBXModel& FBXmodel, std::vector<Ref<Material>>& outMaterials)
{
	for (size_t i = 0; i < FBXmodel.Meshes.size(); i++)
	{
		Ref<Material> tempMaterial = Material::Create(FBXmodel.Meshes[i].MaterialName);

		std::string matName = FBXmodel.Meshes[i].MaterialName;
		std::string DiffusePath = path.parent_path().string() + "/" + matName + "_C.dds";
		std::string NormalPath = path.parent_path().string() + "/" + matName + "_N.dds";
		std::string SpecularPath = path.parent_path().string() + "/" + matName + "_M.dds";
		tempMaterial->SetShader(FBXmodel.Meshes[i].MaterialName);
		if (std::filesystem::exists(DiffusePath))
		{
			tempMaterial->AddDiffuseTexture(Texture2D::Create(DiffusePath));
		}
		if (std::filesystem::exists(NormalPath))
		{
			tempMaterial->AddNormalTexture(Texture2D::Create(NormalPath));
		}
		if (std::filesystem::exists(SpecularPath))
		{
			tempMaterial->AddSpecularTexture(Texture2D::Create(SpecularPath));
		}
		if (!myMaterialSet[tempMaterial->m_ShaderKey])
		{
			myMaterialSet[tempMaterial->m_ShaderKey] = true;
			outMaterials.push_back(tempMaterial);
		}
		else
		{
			continue;
		}
	}
}

void Engine::Mesh::LoadMesh(const std::filesystem::path& aPath, std::vector<Ref<Material>>& outMaterials)
{
	TGA::FBXModel FBXmodel;
	TGA::FBXImporter::LoadModel(aPath.string(), FBXmodel);
	ProcessMaterials(aPath, FBXmodel, outMaterials);
	for (size_t i = 0; i < FBXmodel.Meshes.size(); i++)
	{
		std::vector<Vertex> vertex;
		std::vector<DWORD> index;

		for (size_t j = 0; j < FBXmodel.Meshes[i].Vertices.size(); j++)
		{
			Vertex v;
			memcpy(&v, &FBXmodel.Meshes[i].Vertices[j], sizeof(Vertex));
			vertex.emplace_back(v);
		}

		for (size_t j = 0; j < FBXmodel.Meshes[i].Indices.size(); j++)
		{
			index.emplace_back(FBXmodel.Meshes[i].Indices[j]);
		}
		if (FBXmodel.Meshes[i].MaterialIndex >= outMaterials.size())
		{
			m_SubMeshes.emplace_back(vertex, index, outMaterials[0]);
		}
		else
		{
			m_SubMeshes.emplace_back(vertex, index, outMaterials[FBXmodel.Meshes[i].MaterialIndex]);
		}
	}
}
