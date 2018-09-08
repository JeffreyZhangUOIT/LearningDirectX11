#include "RacingMap.h"

RacingMap::RacingMap() 
{
	y1 = -1.0f;
	y2 = 1.0f;
}

RacingMap::~RacingMap() 
{
	m_pVertexBuffer->Release();
	mIB->Release();
}

// Draws the two racing minigame maps, potentially with different textures.
void RacingMap::draw(Renderer& ren, TextureManager& tex)
{
	auto deviceContext = ren.getDeviceContext();
	tex.changeTex(ren, 5);

	// Update vertex buffer
	D3D11_MAPPED_SUBRESOURCE resource;
	ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, map1, sizeof(map1));
	ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	
	// Draw
	deviceContext->DrawIndexed(6, 0, 0);

	// Change texture
	tex.changeTex(ren, 5);

	// Update vertex buffer
	ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, map2, sizeof(map2));
	ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}

// Scrolls the maps down based on the time since last frame.
void RacingMap::update(float displacement) 
{
	if (displacement <= 0.01f) {
		y1 -= 0.01f;
		y2 -= 0.01f;
	}
	else {
		y1 -= displacement;
		y2 -= displacement;
	}
	if (y1 <= -3) {
		y1 = y2 + 2;
	}
	if (y2 <= -3) {
		y2 = y1 + 2;
	}
	map1[0].y = y1 + 2;
	map1[1].y = y1 + 2;
	map1[2].y = y1;
	map1[3].y = y1;

	map2[0].y = y2 + 2;
	map2[1].y = y2 + 2;
	map2[2].y = y2;
	map2[3].y = y2;
}

void RacingMap::createMesh(Renderer& ren) {
	// Create our vertext buffer

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(Renderer::Vertex) * 4, D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = map1;
	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

	// Create our indicies buffer be careful of back-face culling.
	DWORD indices[6] =
	{
		0, 1, 2,
		1, 3, 2
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DWORD) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Specify the data to initialize the index buffer.
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// Create the index buffer.
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &mIB);
	ren.getDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
}