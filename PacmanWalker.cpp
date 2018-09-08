#include "PacmanWalker.h"

Pacman::Pacman()
{

}

Pacman::~Pacman()
{

}
void Pacman::update(PacmanTiles& pacTiles, float displacement)
{
	// Center the MC, update power up durations, and move the player towards the their destination.
	
	if (pacTiles.checkSquareArea(((std::floor(destX / 0.0625f)) *0.0625f) + 0.06f, destY) == true)
	{
		destX = ((std::floor(destX / 0.0625f)) *0.0625f) + 0.06f;
	}

/*
if (pacTiles.checkSquareArea(destX, ((std::floor(destY / 0.0689655f)) *0.0689655f) + 0.06f) == true)
{
destY = ((std::floor(destY / 0.0689655f)) *0.0689655f) + 0.06f;
}
*/
	
	
	if (speedTicker > 0)
	{
		speedTicker -= displacement;
	}
	if (rangeTicker > 0)
	{
		rangeTicker -= displacement;
	}
	if (speedTicker <= 0) 
	{
		speedModifer = 1;
	}
	if (rangeTicker <= 0)
	{
		rangeModifer = 1;
	}
	
	if (x < destX) {
		x += 0.5f * displacement * speedModifer;
	}

	if (x > destX) {
		x -= 0.5f * displacement * speedModifer;
	}

	if (y < destY) {
		y += 0.8125f * displacement * speedModifer;
	}

	if (y > destY) {
		y -= 0.8125f * displacement * speedModifer;
	}

	// Chart a course for a new destination if we've arrived at previous destination. 
	if ((fabs(x - destX) < 0.05f) & (fabs(y - destY) < 0.05f))
	{
		// Move up
		if (direction == 0) 
		{
			if (y > 1.075f) 
			{
				y = -1.075f;
			}
			if (pacTiles.checkSquareArea(x, y + 0.0689655f) == true)
			{
				destY = y + 0.0689655f;
				destX = x;
			}
			// Hit wall, so do nothing.
		}

		// Move right
		if (direction == 1)
		{
			if (pacTiles.checkSquareArea(x + 0.0625f, y) == true)
			{
				destX = x + 0.0625f;
				destY = y;
			}
			// Hit wall, so do nothing.
		}

		// Move down
		if (direction == 2)
		{

			if (y < -1.075f)
			{
				y = 1.075f;
			}
			if (pacTiles.checkSquareArea(x, y - 0.0689655f) == true)
			{
				destY = y - 0.0689655f;
				destX = x;
			}
			else 
			{
				if (pacTiles.checkSquareArea(x, y) == true)
				{
					destY = ((std::floor(y / 0.0689655f)) *0.0689655f) + 0.01f;
					if (y < 0.9f)
					{
						direction = -1;
					}
				}
			}
			// Hit wall, so do nothing.
			
		}

		// Move left
		if (direction == 3)
		{
			if (pacTiles.checkSquareArea(x - 0.0625f, y) == true)
			{
				destX = x - 0.0625f;
				destY = y;
			}
			// Hit wall, so do nothing.
		}
	}

	// Check if intersection, if so change direction according to player's commands
	// 'W' is down.
	if ((GetKeyState(0x57) & 0x80) != 0)
	{
		if (pacTiles.checkSquareArea(x, y + 0.0689655f) == true)
		{
			direction = 0;
			destX = x;
			destY = y + 0.0689655f;
		}
	}

	// 'S' is down.
	if ((GetKeyState(0x53) & 0x80) != 0)
	{
		if (pacTiles.checkSquareArea(x, y - 0.0689655f) == true)
		{
			direction = 2;
			destX = x;
			destY = y - 0.0689655f;
		}
	}

	// 'A' is down.
	if ((GetKeyState(0x41) & 0x80) != 0)
	{
		if (pacTiles.checkSquareArea(x - 0.0625f, y) == true)
		{
			direction = 3;
			destY = y;
			destX = x - 0.0625f;
		}
	}

	// 'D' is down.
	if ((GetKeyState(0x44) & 0x80) != 0)
	{
		if (pacTiles.checkSquareArea(x + 0.0625f, y) == true)
		{
			direction = 1;
			destY = y;
			destX = x + 0.0625f;
		}
	}
}

void Pacman::draw(Renderer& ren, TextureManager& tex)
{
	auto deviceContext = ren.getDeviceContext();

	// Draw background
	tex.changeTex(ren, 1);

	Renderer::Vertex vertexBuffer[4] =
	{
		{ -1, 1, 0, 0, 0 },
		{ -1, -1, 0, 0, 1 },
		{ 1, 1, 0, 1, 0 },
		{ 1, -1, 0, 1, 1 }
	};

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	D3D11_MAPPED_SUBRESOURCE resource;
	deviceContext->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(mVB, 0);

	deviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);

	
	// Draw player
	tex.changeTex(ren, 0);
	vertexBuffer[0] = { x - 0.05f, y + 0.05f, 0, 0, 0 };
	vertexBuffer[1] = { x - 0.05f, y - 0.05f, 0, 0, 1 };
	vertexBuffer[2] = { x + 0.05f, y + 0.05f, 0, 1, 0 };
	vertexBuffer[3] = { x + 0.05f, y - 0.05f, 0, 1, 1 };

	// Update vertex and pixel buffers
	deviceContext->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(mVB, 0);

	deviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}

void Pacman::init()
{
	direction = 0;
	x = -0.07f; y = 0.0f;
	destX = x; destY = y;
	speedModifer = 1;
	rangeModifer = 1;
	speedTicker = 0;
	rangeTicker = 0;
}

void Pacman::createMesh(Renderer& ren)
{
	Renderer::Vertex m_pVertices[4] =
	{
		{ 1, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 1, 1 }
	};

	// Create our vertext buffer
	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(m_pVertices), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = m_pVertices;

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mVB);

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
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &mIB);
	ren.getDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

}

void Pacman::modSpeed(bool fast)
{
	if (fast)
	{
		speedModifer = 2.0f;
	}
	else
	{
		speedModifer = 0.5f;
	}
	speedTicker = 100;
}