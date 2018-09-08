#include "BackgroundManager.h"

Background::Background(Renderer& ren)
{
	background[0] = { -1, 1, 0.0f, 0, 0 };
	background[1] = { -1, -1, 0.0f, 0, 1 };
	background[2] = { 1, 1, 0.0f, 1, 0 };
	background[3] = { 1, -1, 0.0f, 1, 1 };

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(background), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = background;

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mVB);

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
Background::~Background() 
{

}
void Background::setBackground(int bg)
{
	// Fix this later to support multiple backgrounds.
	currentBackground = bg;
	switch (currentBackground) 
	{
	case 0:
		background[0].Xtex = currentBackground * 0.25f;
		background[1].Xtex = currentBackground * 0.25f;
		background[2].Xtex = (currentBackground + 1) *0.25f;
		background[3].Xtex = (currentBackground + 1) *0.25f;
		background[0].Ytex = 0;
		background[1].Ytex = 0.5f;
		background[2].Ytex = 0;
		background[3].Ytex = 0.5f;
		break;

	case 1:
		background[0].Xtex = currentBackground * 0.25f;
		background[1].Xtex = currentBackground * 0.25f;
		background[2].Xtex = (currentBackground + 1) *0.25f;
		background[3].Xtex = (currentBackground + 1) *0.25f;
		background[0].Ytex = 0;
		background[1].Ytex = 0.5f;
		background[2].Ytex = 0;
		background[3].Ytex = 0.5f;
		break;

	case 2:
		background[0].Xtex = currentBackground * 0.25f;
		background[1].Xtex = currentBackground * 0.25f;
		background[2].Xtex = (currentBackground + 1) *0.25f;
		background[3].Xtex = (currentBackground + 1) *0.25f;
		background[0].Ytex = 0;
		background[1].Ytex = 0.5f;
		background[2].Ytex = 0;
		background[3].Ytex = 0.5f;
		break;

	case 3:
		background[0].Xtex = currentBackground * 0.25f;
		background[1].Xtex = currentBackground * 0.25f;
		background[2].Xtex = (currentBackground + 1) *0.25f;
		background[3].Xtex = (currentBackground + 1) *0.25f;
		background[0].Ytex = 0;
		background[1].Ytex = 0.5f;
		background[2].Ytex = 0;
		background[3].Ytex = 0.5f;
		break;

	case 4:
		background[0].Xtex = (currentBackground%4) * 0.25f;
		background[1].Xtex = (currentBackground % 4) * 0.25f;
		background[2].Xtex = ((currentBackground % 4) + 1) *0.25f;
		background[3].Xtex = ((currentBackground % 4) + 1) *0.25f;
		background[0].Ytex = 0.5f;
		background[1].Ytex = 1.0f;
		background[2].Ytex = 0.5f;
		background[3].Ytex = 1.0f;
		break;
	case 5:
		background[0].Xtex = (currentBackground % 4) * 0.25f;
		background[1].Xtex = (currentBackground % 4) * 0.25f;
		background[2].Xtex = ((currentBackground % 4) + 1) *0.25f;
		background[3].Xtex = ((currentBackground % 4) + 1) *0.25f;
		background[0].Ytex = 0.5f;
		background[1].Ytex = 1.0f;
		background[2].Ytex = 0.5f;
		background[3].Ytex = 1.0f;
		break;
	case 6:
		background[0].Xtex = (currentBackground % 4) * 0.25f;
		background[1].Xtex = (currentBackground % 4) * 0.25f;
		background[2].Xtex = ((currentBackground % 4) + 1) *0.25f;
		background[3].Xtex = ((currentBackground % 4) + 1) *0.25f;
		background[0].Ytex = 0.5f;
		background[1].Ytex = 1.0f;
		background[2].Ytex = 0.5f;
		background[3].Ytex = 1.0f;
		break;
	case 7:
		background[0].Xtex = (currentBackground % 4) * 0.25f;
		background[1].Xtex = (currentBackground % 4) * 0.25f;
		background[2].Xtex = ((currentBackground % 4) + 1) *0.25f;
		background[3].Xtex = ((currentBackground % 4) + 1) *0.25f;
		background[0].Ytex = 0.5f;
		background[1].Ytex = 1.0f;
		background[2].Ytex = 0.5f;
		background[3].Ytex = 1.0f;
		break;
	}
}

void Background::draw(Renderer& ren, TextureManager& tex)
{
	tex.changeTex(ren, 13);

	// Setup vertex buffer.
	auto deviceContext = ren.getDeviceContext();
	D3D11_MAPPED_SUBRESOURCE resource;
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, background, sizeof(background));
	deviceContext->Unmap(mVB, 0);

	// Bind our vertex buffer
	stride = sizeof(Renderer::Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);

}