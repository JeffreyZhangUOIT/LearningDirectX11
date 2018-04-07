#include "Bullet.h"

Bullet::Bullet(float* pos, Renderer& ren) 
{
	x = pos[0];
	y = pos[1];
	point = new float[2];
	point[0] = x;
	point[1] = y;
	dir = pos[2];

	// Move the bullet out of the player, without drawing it. Helps make it seem like it's coming out of the right place.
	update();
	update();
	update();
	
}

Bullet::Bullet(const Bullet &bul) {

	x = bul.x;
	y = bul.y;
	dir = bul.dir;
	point = nullptr;
	point =  new float[2];
	point[0] = x;
	point[1] = y;
}

Bullet::~Bullet() 
{

}

Bullet::ArrVer Bullet::update()
{
	OOB = false;
	if ((dir > 0) & (dir < 0.5f* 3.14f)) {
		y += 0.03f * cosf(fabs(dir));
	}
	if (dir > 0.5f*3.14) {
		y -= 0.03f * -cosf(fabs(dir));
	}
	if ((dir < 0) & (dir > -0.5f*3.14)) {
		y += 0.03f * cosf(fabs(dir));
	}
	if (dir < -0.5f*3.14) {
		y -= 0.03f * -cosf(fabs(dir));
	}
	if ((dir > 0) & (dir < 0.5f* 3.14f)) {
		x += 0.03f * sinf(fabs(dir));
	}
	if (dir > 0.5f*3.14) {
		x += 0.03f * sinf(fabs(dir));
	}
	if ((dir < 0) & (dir > -0.5f*3.14)){
		x -= 0.03f * sinf(fabs(dir));
	}
	if (dir < -0.5f*3.14) {
		x -= 0.03f * sinf(fabs(dir));
	}
	if (fabs(x) > 1.2f || fabs(y) > 1.2f) {
		OOB = true;
	}

	DirectX::XMFLOAT4 p1, p2, p3, p4;
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMVECTOR translate = DirectX::XMVectorSet(x, y, 0, 0);
	DirectX::XMVECTOR RotationQuaternion = DirectX::XMQuaternionRotationAxis({ 0, 0, -1, 0 }, dir);
	DirectX::XMVECTOR RotationCenter = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMMATRIX transform = DirectX::XMMatrixAffineTransformation(scale, RotationCenter, RotationQuaternion, translate);

	DirectX::XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, transform);
	DirectX::XMVECTOR v1 = DirectX::XMVectorSet(-0.01f, -0.01f, 0, 1);
	DirectX::XMVECTOR v2 = DirectX::XMVectorSet(0.01f, -0.01f, 0, 1);
	DirectX::XMVECTOR v3 = DirectX::XMVectorSet(-0.01f, 0.01f, 0, 1);
	DirectX::XMVECTOR v4 = DirectX::XMVectorSet(0.01f, 0.01f, 0, 1);

	v1 = DirectX::XMVector4Transform(v1, transform);
	v2 = DirectX::XMVector4Transform(v2, transform);
	v3 = DirectX::XMVector4Transform(v3, transform);
	v4 = DirectX::XMVector4Transform(v4, transform);

	DirectX::XMStoreFloat4(&p1, v1);
	DirectX::XMStoreFloat4(&p2, v2);
	DirectX::XMStoreFloat4(&p3, v3);
	DirectX::XMStoreFloat4(&p4, v4);

	Bullet::ArrVer newVertices;
	newVertices.vertices[0] = { p1.x, p1.y, 0, 1, 0.7176f, 0.1176f, 1 };
	newVertices.vertices[1] = { p2.x, p2.y, 0, 1, 0.7176f, 0.1176f, 1 };
	newVertices.vertices[2] = { p3.x, p3.y, 0, 1, 0.7176f, 0.1176f, 1 };
	newVertices.vertices[3] = { p4.x, p4.y, 0, 1, 0.7176f, 0.1176f, 1 };

	return newVertices;
}

float * Bullet::getPos() 
{
	if (point) 
	{
		point[0] = x;
		point[1] = y;
		return point;
	}
	else
	{
		point = new float[2];
		point[0] = x;
		point[1] = y;
		return point;
	}
}

/*
{
{ p1.x, p1.y, 0, 1, 0.7176f, 0.1176f, 1 },
{ p2.x, p2.y, 0, 1, 0.7176f, 0.1176f, 1 },
{ p3.x, p3.y, 0, 1, 0.7176f, 0.1176f, 1 },
{ p4.x, p4.y, 0, 1, 0.7176f, 0.1176f, 1 }
}
void Bullet::draw(Renderer& renderer) 
{
	auto deviceContext = renderer.getDeviceContext();

	if (m_pVertexBuffer == nullptr) {
		createMesh(renderer);
	}

	// Update vertex and pixel buffers
	D3D11_MAPPED_SUBRESOURCE resource;
	if (!m_pVertexBuffer) {
		createMesh(renderer);
	}

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


void Bullet::createMesh(Renderer& ren)
{
// Create our vertext buffer
auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(m_pVertices), D3D11_BIND_VERTEX_BUFFER);
vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
D3D11_SUBRESOURCE_DATA vertexData = { 0 };
vertexData.pSysMem = m_pVertices;

ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);


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