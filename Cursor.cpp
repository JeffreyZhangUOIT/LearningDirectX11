/*
Copyright 2017, 2018 Jeffrey Zhang

This file is part of ProjectFiasco.

ProjectFiasco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ProjectFiasco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ProjectFiasco.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Cursor.h"

Mouse::Mouse(Renderer& ren){
	createMesh(ren);
	x = 0;
	y = 0;
}

Mouse::~Mouse(){
	m_pVertexBuffer->Release();
	mIB->Release();
}

void Mouse::update(Renderer& renderer, float* aim) {
	
	Renderer::Vertex vertices[] = {
	{ aim[0]-0.01f, aim[1]-0.01f, 0.001f, 0, 1 },
	{ aim[0]+0.01f, aim[1]-0.01f, 0.001f, 1, 1 },
	{ aim[0]-0.01f, aim[1]+0.01f, 0.001f, 0, 0 },
	{ aim[0]+0.01f, aim[1]+0.01f, 0.001f, 1, 0 }
	};

	D3D11_MAPPED_SUBRESOURCE resource;
	renderer.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertices, sizeof(vertices));
	renderer.getDeviceContext()->Unmap(m_pVertexBuffer, 0);

}

void Mouse::createMesh(Renderer& ren) {
	// Create our vertext buffer
	Renderer::Vertex vertices[] = {
	{ -0.01f, -0.01f, 0.001f, 0, 0},
	{ +0.01f, -0.01f, 0.001f, 1, 0 },
	{ -0.01f, 0.01f, 0.001f, 0, 1 },
	{ 0.01f, 0.01f, 0.001f, 1, 1}
	};

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(Renderer::Vertex)*4, D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = vertices;
	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

	// Create our indicies buffer be careful of back-face culling.
	DWORD indices[6] =
	{
		2, 1, 0,
		1, 2, 3
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

void Mouse::draw(Renderer& ren, TextureManager& tex) {
	auto deviceContext = ren.getDeviceContext();

	tex.changeTex(ren, 2);

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}