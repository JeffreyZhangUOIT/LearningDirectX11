#include "PacmanTiles.h"

PacmanTiles::PacmanTiles()
{
	int i = 0;
	while (i < 928)
	{
		board[i] = 0;
		i++;
	}
}

PacmanTiles::~PacmanTiles(){}

void PacmanTiles::initBG1(Renderer& ren)
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


	fopen_s(&boardInfo, "PacmanBoard.txt", "r+");
	if (boardInfo == NULL)
	{
		fopen_s(&boardInfo, "PacmanBoard.txt", "w+");
	}
	if (boardInfo != NULL)
	{
		char c;
		int i = 0;

		do
		{
			c = fgetc(boardInfo);
			board[i] = c - '0';
			i++;
		} while ((c != EOF) || (i < 928));
	}

	if (boardInfo != NULL)
	{
		//fclose(boardInfo);
	}
	
}

bool PacmanTiles::checkSquareArea(float x, float y)
{
	x = x + 1.00f;
	y = y + 1.00f;
	if (y >= 1.97f)
	{
		y = 1.9f;
	}

	if (y <= 0.03f)
	{
		y = 0.06f;
	}
	int leftSquare =(x / 0.0625f);
	int bottomSquare = (y / 0.0689655f);
	int bottemLeft = leftSquare + (32 * bottomSquare);
	if (bottemLeft < 895) 
	{
		if ((board[bottemLeft] == 0) & (board[bottemLeft + 1] == 0) & (board[bottemLeft + 32] == 0) & (board[bottemLeft + 33] == 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	
}

bool PacmanTiles::checkTile(float x, float y)
{
	x = x + 1.00f;
	y = y + 1.00f;
	if (y >= 1.97f)
	{
		y = 1.9f;
	}

	if (y <= 0.03f)
	{
		y = 0.06f;
	}
	int leftSquare = (x / 0.0625f);
	int bottomSquare = (y / 0.0689655f);
	int bottemLeft = leftSquare + (32 * bottomSquare);
	if (bottemLeft < 895)
	{
		if (board[bottemLeft] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}


void PacmanTiles::drawBoard(Renderer& ren, TextureManager& tex)
{
	auto deviceContext = ren.getDeviceContext();
	int i = 0;
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;
	D3D11_MAPPED_SUBRESOURCE resource;
	Renderer::Vertex m_pVertices[4] =
	{
		{ 1, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 1, 1 }
	};

	tex.changeTex(ren, -1);
	while (i < 928)
	{
		if (board[i] == 1)
		{
			m_pVertices[0] = { (((i % 32))*0.0625f) - 1.0f, (((i / 32)+1)*0.0689655f) - 1.0f, 0, 0, 0 };
			m_pVertices[1] = { (((i % 32))*0.0625f) - 1.0f, (((i / 32))*0.0689655f) - 1.0f, 0, 0, 1 };
			m_pVertices[2] = { (((i % 32)+ 1)*0.0625f) - 1.0f, (((i / 32)+1)*0.0689655f) - 1.0f, 0, 1, 0 };
			m_pVertices[3] = { (((i % 32)+ 1)*0.0625f) - 1.0f, (((i / 32))*0.0689655f) - 1.0f, 0, 1, 1 };

			deviceContext->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

			memcpy(resource.pData, m_pVertices, sizeof(m_pVertices));
			deviceContext->Unmap(mVB, 0);

			deviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
			deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

			// Draw
			deviceContext->DrawIndexed(6, 0, 0);
		}
		i++;
	}
}