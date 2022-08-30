#pragma once
#include <d3d11.h>
#include <string>
#include <vector>
#include "ModelData.hpp"
#include "Engine/DX11/DX11.h"
#include "Engine/Utils/Utilities.h"
#include "Engine/Renderer/Buffers/IndexBuffer.h"
#include "Engine/Renderer/Buffers/VertexBuffer.h"
#include "Engine/Assets/Material/Material.h"


namespace Engine
{
	class SubMesh
	{
	public:
		SubMesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices, Ref<Material> material);
		SubMesh(const SubMesh& aMesh);
		void Draw();
	private:
		Ref<Material> m_Material;
		VertexBuffer<Vertex> myVertexBuffer;
		IndexBuffer myIndexBuffer;
	};
}