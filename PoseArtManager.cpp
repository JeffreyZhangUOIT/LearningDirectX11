#include "PoseArtManager.h"

PoseArtManager::PoseArtManager(Renderer& ren) 
{
	x1 = -1.5f; x2 = 1.5f;
	y1 = 0; y2 = 0;
	g1x = -0.8f; g2x = 0.8f;
	q1 = 7; q2 = 7;
	t1X = 0; t1Y = 0;
	t2X = 0; t2Y = 0;
	enterLeft = false;
	enterRight = false;
	leftOldest = true;
	createMesh(ren);
}

PoseArtManager::~PoseArtManager() 
{

}
void PoseArtManager::update() 
{
	if (x1 < g1x) {
		x1 += 0.013f;
	}
	if (y1 < g1y) {
		y1 += 0.013f;
	}
	if (x2 < g2x) {
		x2 += 0.013f;
	}
	if (y2 < g2y) {
		y2 += 0.013f;
	}
	if (x1 > g1x) {
		x1 -= 0.013f;
	}
	if (y1 > g1y) {
		y1 -= 0.013f;
	}
	if (x2 > g2x) {
		x2 -= 0.013f;
	}
	if (y2 > g2y) {
		y2 -= 0.013f;
	}

	if ((fabs(x1 - g1x) < 0.005f) & (fabs(y1 - g1y) < 0.005f) & enterLeft) 
	{
		t1X = (q1%4) * 0.25f;
		g1x = -0.8f;
		enterLeft = false;
		if (q1 <= 3)
		{
			p1t = 18;
		}
		else
		{
			p1t = 19;
		}
	}
	if ((fabs(g2x - x2) < 0.005f) & (fabs(g2y - y2) < 0.005f) & enterRight)
	{
		t2X = (q2 % 4) * 0.25f;
		g2x = 0.8f;
		enterRight = false;
		if (q2 <= 3)
		{
			p2t = 18;
		}
		else
		{
			p2t = 19;
		}
	}
}

void PoseArtManager::draw(Renderer& ren, TextureManager& tex) 
{
	
	// Takes up 6/10 of the screen vertically, and 1/4 of the screen horizontally per person.
	box1[0] = { x1 - 0.25f, y1 + 0.7f, 0.0f, t1X, t1Y};
	box1[1] = { x1 - 0.25f, -1.0f, 0.0f, t1X, t1Y + 1.0f};
	box1[2] = { x1 + 0.25f, y1 + 0.7f, 0.0f, t1X+0.25f, t1Y};
	box1[3] = { x1 + 0.25f, -1.0f, 0.0f, t1X+0.25f, t1Y+1.0f};

	box2[0] = { x2 - 0.25f, y2 + 0.7f, 0.0f, t2X, t2Y };
	box2[1] = { x2 - 0.25f, -1.0f, 0.0f, t2X, t2Y + 1.0f };
	box2[2] = { x2 + 0.25f, y2 + 0.7f, 0.0f, t2X+0.25f, t2Y };
	box2[3] = { x2 + 0.25f, -1.0f, 0.0f, t2X+0.25f, t2Y + 1.0f };

	auto deviceContext = ren.getDeviceContext();

	// Update vertex buffer.
	tex.changeTex(ren, p1t);
	D3D11_MAPPED_SUBRESOURCE resource;
	
	deviceContext->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, box1, sizeof(box1));
	deviceContext->Unmap(mVB, 0);

	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);

	tex.changeTex(ren, p2t);
	deviceContext->Map(mVB2, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, box2, sizeof(box2));
	deviceContext->Unmap(mVB2, 0);

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->IASetVertexBuffers(0, 1, &mVB2, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);
}

void PoseArtManager::enter(int person)
{
	// 00 = Toji, 10 = Mifune, 20 = Inori, 30 = Akari, 40 = Emilia, 50 = ???, 60 = Anon Faculty, 70 = Vice Principal
	// 00-09 all belong to Toji, allowing different stances of BROKEN to just flash onscreen.
	// Shifts out the oldest standing person with another person.
	if (((person/10) != q1) & ((person/10) != q2)) 
	{
		if (leftOldest)
		{
			g1x = -1.5f;
			enterLeft = true;
			leftOldest = false;
			q1 = person / 10;
			
		}
		else
		{
			g2x = 1.5f;
			enterRight = true;
			leftOldest = true;
			q2 = person / 10;
		}
	}

	else 
	{
		if ((person % 10) == q1) 
		{
			// Edit this to change # of poses per character. Currently supports 1. 
			t1Y = (person % 10);
		}
		if ((person % 10) == q2)
		{
			// Edit this to change # of poses per character. Currently supports 1. 
			t2Y = (person % 10);
		}
	}
	

}
void PoseArtManager::exit(int person)
{
	// Causes the person (if they are currently on screen) to be moved off screen, and switched with a transparent background.
	if (q1 == (person/10)) {
		g1x = -1.3f;
		q1 = 7;
		leftOldest = true;
	}

	if (q2 == (person / 10)) {
		g2x = 1.3f;
		q2 = 7;
		leftOldest = false;
	}
}

void PoseArtManager::exitAll() {
	g1x = -1.3f;
	g2x = 1.3f;
	x1 = g1x;
	x2 = g2x;
	q1 = 7;
	q2 = 7;
	leftOldest = true;
}
void PoseArtManager::createMesh(Renderer& ren)
{
	// Create our vertext buffer
	Renderer::Vertex vertices[4] =
	{
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

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mVB);
	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mVB2);

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