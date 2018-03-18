#include "Player.h"
#include <DirectXMath.h>

using namespace std;

struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

void Player::init() {
	playerHp = 3;
	immunity = 0;
}
Player::Player(Renderer& ren) {
	createMesh(ren);
	init();
}

Player::~Player() {
	m_pVertexBuffer->Release();
}

void Player::update(Renderer& renderer, float * pos, bool dmg) {

	if (dmg) {
		if (immunity <= 0) {
			playerHp--;
			immunity = 100;
		}
	}

	if (immunity > 0) {
		immunity--;
	}
	if (playerHp <= 0) {
		alive = false;

	}

	else {
		alive = true;
	}

	//x = 0.5f; y = 0.5f;
	DirectX::XMFLOAT4 p1, p2, p3;
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMVECTOR translate = DirectX::XMVectorSet(pos[0], pos[1], 0, 0);
	DirectX::XMVECTOR RotationQuaternion = DirectX::XMQuaternionRotationAxis({ 0, 0, -1, 0 }, pos[2]);
	DirectX::XMVECTOR RotationCenter = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMMATRIX transform = DirectX::XMMatrixAffineTransformation(scale, RotationCenter, RotationQuaternion, translate);
	
	DirectX::XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, transform);
	DirectX::XMVECTOR v1 = DirectX::XMVectorSet(-0.05f, -0.05f, 0, 1 );
	DirectX::XMVECTOR v2 = DirectX::XMVectorSet( 0, 0.15f, 0, 1 );
	DirectX::XMVECTOR v3 = DirectX::XMVectorSet( 0.05f, -0.05f, 0, 1 );

	v1 = DirectX::XMVector4Transform(v1, transform);
	//DirectX::XMVector4Transform(v1, translate);
	v2 = DirectX::XMVector4Transform(v2, transform);
	//DirectX::XMVector4Transform(v2, translate);
	v3 = DirectX::XMVector4Transform(v3, transform);
	//DirectX::XMVector4Transform(v3, translate);

	DirectX::XMStoreFloat4(&p1, v1);
	DirectX::XMStoreFloat4(&p2, v2);
	DirectX::XMStoreFloat4(&p3, v3);

	Vertex vertices[] = {
	{ p1.x, p1.y, 0, 1, 1, 1, 1 },
	{ p2.x, p2.y, 0, 1, 1, 1, 1 },
	{ p3.x, p3.y, 0, 1, 1, 1, 1 },
	};
	if (immunity > 0) {
		vertices[0].g = 0.5f;
		vertices[0].b = 0.5f;
		vertices[1].g = 0.5f;
		vertices[1].b = 0.5f;
		vertices[2].g = 0.5f;
		vertices[2].b = 0.5f;

	}


	D3D11_MAPPED_SUBRESOURCE resource;
	renderer.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertices, sizeof(vertices));
	renderer.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
	
}

void Player::draw(Renderer& renderer) {
	auto deviceContext = renderer.getDeviceContext();

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Draw
	deviceContext->Draw(3, 0);

}

void Player::createMesh(Renderer& renderer) {
	// Create our vertext buffer
	Vertex vertices[] = {
		{ -1, -1, 0, 1, 0, 0, 1 },
		{ 0, 1, 0, 0, 1, 0, 1 },
		{ 1, -1, 0, 0, 0, 1, 1 },
	};

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = vertices;

	renderer.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

}
