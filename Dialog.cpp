#include "Dialog.h"

struct Vertex
{
	float x, y, z;
	float r, g, b, a;
};

Dialog::Dialog(Renderer& ren) {
	createMesh(ren);
}

Dialog::~Dialog() {
	//delete mIB;
	//delete m_pVertexBuffer;
}

void Dialog::update(Renderer& ren, float* boxDimen) {
	Vertex vertices[] = {
	{ boxDimen[2], boxDimen[3], 0, 1, 1, 1, 0.4f },
	{ boxDimen[0], boxDimen[3], 0, 1, 1, 1, 0.4f },
	{ boxDimen[2], boxDimen[1], 0, 1, 1, 1, 0.4f },
	{ boxDimen[0], boxDimen[1], 0, 1, 1, 1, 0.4f }
	};

	D3D11_MAPPED_SUBRESOURCE resource;
	ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertices, sizeof(vertices));
	ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);

	DWORD indices[6] =
	{
		2, 1, 0,
		1, 2, 3
	};


	ren.getDeviceContext()->Map(mIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, indices, sizeof(indices));
	ren.getDeviceContext()->Unmap(mIB, 0);
}

void Dialog::draw(Renderer& ren) {
	auto deviceContext = ren.getDeviceContext();

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


void Dialog::createMesh(Renderer& ren) {

	// Create our vertext buffer
	Vertex vertices[] = {
	{ -0.90f, -0.90f, 0, 1, 1, 1, 0.4f },
	{ 0.90f, -0.90f, 0, 1, 1, 1, 0.4f },
	{ 0.90f, -0.50f, 0, 1, 1, 1, 0.4f },
	{ -0.90f, -0.50f, 0, 1, 1, 1, 0.4f }
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