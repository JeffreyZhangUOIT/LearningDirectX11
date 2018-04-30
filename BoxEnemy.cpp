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
#include "BoxEnemy.h"

BoxEnemy::BoxEnemy(float posx, float posy, Renderer& ren) {
	x = posx;
	y = posy;
	EnemyHp = 3;
	touch = false;
	slope = 0;
}

BoxEnemy::BoxEnemy(const BoxEnemy &b2) {
	x = b2.x;
	y = b2.y;
	EnemyHp = b2.EnemyHp;
	touch = b2.touch;
	slope = b2.slope;
}

bool BoxEnemy::bulletCollision(float* pos) 
{
	if ((fabs(y - pos[1]) < 0.05f) && (fabs(x - pos[0]) < 0.05f))
	{
		EnemyHp--;
		return true;
	}

	return false;
}
BoxEnemy::~BoxEnemy() {

}
float* BoxEnemy::getPos() 
{
	float point[2] = { x, y };
	return point;
}
BoxEnemy::ArrVer BoxEnemy::update(float* playerPos) {
	float hpPercent = (EnemyHp / 5.0f);
	slope = atan2((playerPos[0] - x), (playerPos[1] - y));

	if ((x - playerPos[0]) > 0) {
		x -= 0.005f;
	}
	if ((x - playerPos[0]) < 0) {
		x += 0.005f;
	}
	if ((y - playerPos[1]) > 0) {
		y -= 0.005f;
	}
	if ((y - playerPos[1]) < 0) {
		y += 0.005f;
	}
	
	if ((fabs(y - playerPos[1]) < 0.13f) && (fabs(x - playerPos[0]) < 0.13f)) 
	{
		touch = true;
	}
	else 
	{
		touch = false;
	}
	
	DirectX::XMFLOAT4 p1, p2, p3, p4;
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMVECTOR translate = DirectX::XMVectorSet(x, y, 0, 0);
	DirectX::XMVECTOR RotationQuaternion = DirectX::XMQuaternionRotationAxis({ 0, 0, -1, 0 }, slope);
	DirectX::XMVECTOR RotationCenter = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMMATRIX transform = DirectX::XMMatrixAffineTransformation(scale, RotationCenter, RotationQuaternion, translate);

	DirectX::XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, transform);
	DirectX::XMVECTOR v1 = DirectX::XMVectorSet(-0.05f, -0.05f, 0, 1);
	DirectX::XMVECTOR v2 = DirectX::XMVectorSet(0.05f, -0.05f, 0, 1);
	DirectX::XMVECTOR v3 = DirectX::XMVectorSet(-0.05f, 0.05f, 0, 1);
	DirectX::XMVECTOR v4 = DirectX::XMVectorSet(0.05f, 0.05f, 0, 1);

	v1 = DirectX::XMVector4Transform(v1, transform);
	v2 = DirectX::XMVector4Transform(v2, transform);
	v3 = DirectX::XMVector4Transform(v3, transform);
	v4 = DirectX::XMVector4Transform(v4, transform);

	DirectX::XMStoreFloat4(&p1, v1);
	DirectX::XMStoreFloat4(&p2, v2);
	DirectX::XMStoreFloat4(&p3, v3);
	DirectX::XMStoreFloat4(&p4, v4);

	ArrVer newVertices;
	newVertices.vertices[0] = { p1.x, p1.y, 0, 1, 1, 1, 1 };
	newVertices.vertices[1] = { p2.x, p2.y, 0, 1, 1, 1, 1 };
	newVertices.vertices[2] = { p3.x, p3.y, 0, 1, 1, 1, 1 };
	newVertices.vertices[3] = { p4.x, p4.y, 0, 1, 1, 1, 1 };

	newVertices.vertices[4] = { x - 0.05f, y + 0.06f, 0, 1, 0, 0, 1 };
	newVertices.vertices[5] = { ((x - 0.05f) + (0.17f * hpPercent)), y + 0.06f, 0, 1, 0, 0, 1 };
	newVertices.vertices[6] = { x - 0.05f, y + 0.08f, 0, 1, 0, 0, 1 };
	newVertices.vertices[7] = { ((x - 0.05f) + (0.17f * hpPercent)), y + 0.08f, 0, 1, 0, 0, 1 };

	return newVertices;
}
/*
void BoxEnemy::draw(Renderer& renderer) {
	auto deviceContext = renderer.getDeviceContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	assert(m_pVertexBuffer != NULL);
	deviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, m_pVertices, sizeof(m_pVertices));
	deviceContext->Unmap(m_pVertexBuffer, 0);

	deviceContext->Map(mIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, m_pIndices, sizeof(m_pIndices));
	deviceContext->Unmap(mIB, 0);
	

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);

}
*/
bool BoxEnemy::checkCollision(){
	return touch;
}

/*
void BoxEnemy::createMesh(Renderer& ren) {

	// Create our vertext buffer
	Vertex vertices[] = {
	{ 0, 0, 0, 1, 1, 1, 1 },
	{ 0, 1, 0, 1, 1, 1, 1 },
	{ 1, 0, 0, 1, 1, 1, 1 },
	{ 1, 1, 0, 1, 1, 1, 1 }
	};

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = m_pVertices;

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

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
	iinitData.pSysMem = m_pIndices;

	// Create the index buffer.
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &mIB);
	ren.getDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
}
*/