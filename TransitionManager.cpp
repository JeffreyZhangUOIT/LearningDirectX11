#include "TransitionManager.h"

Transition::Transition(Renderer& ren)
{
	foreground[0] = { -1, 1, 0.0f, 0, 0 };
	foreground[1] = { -1, -1, 0.0f, 0, 1 };
	foreground[2] = { 1, 1, 0.0f, 1, 0 };
	foreground[3] = { 1, -1, 0.0f, 1, 1 };

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(foreground), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = foreground;

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
Transition::~Transition()
{

}

void Transition::update(Background& backdrop, Timer& time)
{
	remainingTime = endTime - time.deltaFT();
	if (changedScene == false)
	{
		
		if (remainingTime <= 0.5)
		{
			backdrop.setBackground(scene);
			changedScene = true;
		}
	}
}

void Transition::draw(Renderer& ren, TextureManager& tex)
{
	if ((remainingTime <= 1.0f) & (remainingTime >= 0.1f))
	{
		foreground[0].Xtex = remainingTime - 0.1f;
		foreground[1].Xtex = remainingTime - 0.1f;
		foreground[2].Xtex = remainingTime;
		foreground[3].Xtex = remainingTime;

		tex.changeTex(ren, 17);

		// Setup vertex buffer.
		auto deviceContext = ren.getDeviceContext();
		D3D11_MAPPED_SUBRESOURCE resource;
		UINT stride = sizeof(Renderer::Vertex);
		UINT offset = 0;

		deviceContext->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		memcpy(resource.pData, foreground, sizeof(foreground));
		deviceContext->Unmap(mVB, 0);

		// Bind our vertex buffer
		stride = sizeof(Renderer::Vertex);
		offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->DrawIndexed(6, 0, 0);
	}
}

void Transition::init(int s, Timer& time)
{
	scene = s;
	endTime = time.deltaFT() + 1.75f;
	changedScene = false;
}