#include "SubMesh.h"

#include <utility>

Engine::SubMesh::SubMesh(std::vector<Vertex>& aVertices, std::vector<DWORD>& aIndices, Ref<Material> material)
{
	myVertexBuffer.Initialize(aVertices.data(), static_cast<UINT>(aVertices.size()));
	myIndexBuffer.Initalize(aIndices.data(), static_cast<UINT>(aIndices.size()));
	m_Material = material;
}

Engine::SubMesh::SubMesh(const SubMesh& aMesh) : myIndexBuffer(aMesh.myIndexBuffer)
{
	myVertexBuffer = aMesh.myVertexBuffer;
	m_Material = aMesh.m_Material;
}

void Engine::SubMesh::Draw()
{
	UINT offset = 0;
	if(m_Material) m_Material->Bind();
	DX11::Context()->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.StridePtr(), &offset);
	DX11::Context()->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	DX11::Context()->DrawIndexed(myIndexBuffer.BufferSize(), 0, 0);
}
