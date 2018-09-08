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
#include "Player.h"
#include <DirectXMath.h>

using namespace std;

void Player::init() {
	immunity = 0;
	
	pPos[0] = 0;
	pPos[1] = 0;
}

Player::Player() {
	init();
}

Player::~Player() {
	//m_pVertexBuffer->Release();
}

float * Player::update(Renderer& renderer, TextureManager& tex, float * pos, bool dmg, int gamemode) {
	// If driving minigame, bind player to the borders of the screen
	pPos[0] = pos[0];
	pPos[1] = pos[1];

	if (gamemode == 1) 
	{
		tex.changeTex(renderer, 0);
	}
	
	if (gamemode == 3) {
		if (pPos[0] < -1.0f) {
			pPos[0] = -1.0f;
		}
		if (pPos[0] > 1.0f) {
			pPos[0] = 1.0f;
		}
		if (pPos[1] > 1.0f) {
			pPos[1] = 1.0f;
		}
		if (pPos[1] < -1.0f) {
			pPos[1] = -1.0f;
		}
		if (pPos[0] < -0.6f) {
			pPos[0] = -0.6f;
		}
		if (pPos[0] > 0.6f) {
			pPos[0] = 0.6f;
		}
		
		tex.changeTex(renderer, 7);
	}

	pos[2] = 0;

	if (dmg) {
		if (immunity <= 0) {
			immunity = 100;
		}
	}

	// Need to change this to time based some day.
	if (immunity > 0) {
		immunity--;
	}

	// Draw the player with the appropriate translation, rotation, and scale.
	DirectX::XMFLOAT4 p1, p2, p3, p4;
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMVECTOR translate = DirectX::XMVectorSet(pPos[0], pPos[1], 0, 0);
	DirectX::XMVECTOR RotationQuaternion = DirectX::XMQuaternionRotationAxis({ 0, 0, -1, 0 }, pos[2]);
	DirectX::XMVECTOR RotationCenter = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMMATRIX transform = DirectX::XMMatrixAffineTransformation(scale, RotationCenter, RotationQuaternion, translate);
	
	DirectX::XMVECTOR v1 = DirectX::XMVectorSet(-0.05f, 0.075f, 0, 1 );
	DirectX::XMVECTOR v2 = DirectX::XMVectorSet(-0.05f, -0.075f, 0, 1 );
	DirectX::XMVECTOR v3 = DirectX::XMVectorSet(0.05f, 0.075f, 0, 1 );
	DirectX::XMVECTOR v4 = DirectX::XMVectorSet(0.05f, -0.075f, 0, 1 );

	v1 = DirectX::XMVector4Transform(v1, transform);
	v2 = DirectX::XMVector4Transform(v2, transform);
	v3 = DirectX::XMVector4Transform(v3, transform);
	v4 = DirectX::XMVector4Transform(v4, transform);
	
	DirectX::XMStoreFloat4(&p1, v1);
	DirectX::XMStoreFloat4(&p2, v2);
	DirectX::XMStoreFloat4(&p3, v3);
	DirectX::XMStoreFloat4(&p4, v4);

	Renderer::Vertex vertices[] = {
		{ p1.x, p1.y, 0, 0, 0 },
		{ p2.x, p2.y, 0, 0, 1 },
		{ p3.x, p3.y, 0, 1, 0 },
		{ p4.x, p4.y, 0, 1, 1 }
	};

	D3D11_MAPPED_SUBRESOURCE resource;
	renderer.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertices, sizeof(vertices));
	renderer.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
	return pPos;
}

void Player::draw(Renderer& renderer) {
	auto deviceContext = renderer.getDeviceContext();

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

void Player::createMesh(Renderer& renderer) {
	// Create our vertext buffer
	Renderer::Vertex vertices[] = {
		{ 1, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 1, 1 }
	};

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = vertices;

	renderer.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

	DWORD m_pIndices[6] =
	{
		2, 1, 0,
		1, 2, 3
	};

	// Create our index buffer
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
	renderer.getDevice()->CreateBuffer(&ibd, &iinitData, &mIB);
	renderer.getDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
}
