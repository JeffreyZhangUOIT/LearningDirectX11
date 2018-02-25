#include "BoxEnemy.h"

BoxEnemy::BoxEnemy(float posx, float posy, Renderer& ren) {
	x = posx;
	y = posy;
	touch = false;
	slope = 0;
	createMesh(ren);
}

BoxEnemy::~BoxEnemy() {
	m_pVertexBuffer->Release();
	mIB->Release();
}

void BoxEnemy::update(Renderer& renderer, float* playerPos) {
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
	
	if ((fabs(y - playerPos[1]) < 0.05f) && (fabs(x - playerPos[0]) < 0.05f)) 
	{
		touch = true;
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

	Vertex vertices[] = {
	{ p1.x, p1.y, 0, 1, 1, 1, 1 },
	{ p2.x, p2.y, 0, 1, 1, 1, 1 },
	{ p3.x, p3.y, 0, 1, 1, 1, 1 },
	{ p4.x, p4.y, 0, 1, 1, 1, 1 }
	};

	D3D11_MAPPED_SUBRESOURCE resource;
	renderer.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertices, sizeof(vertices));
	renderer.getDeviceContext()->Unmap(m_pVertexBuffer, 0);

	DWORD indices[6] =
	{
		2, 1, 0,
		1, 2, 3
	};

	renderer.getDeviceContext()->Map(mIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, indices, sizeof(indices));
	renderer.getDeviceContext()->Unmap(mIB, 0);
}

void BoxEnemy::draw(Renderer& renderer) {
	auto deviceContext = renderer.getDeviceContext();

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

bool BoxEnemy::checkCollision(){
	return touch;
}

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
	vertexData.pSysMem = vertices;

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
	iinitData.pSysMem = indices;

	// Create the index buffer.
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &mIB);
	ren.getDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
}