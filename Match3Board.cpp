#include "Match3Board.h"

M3Board::M3Board() {

}

M3Board::~M3Board() {

}

void M3Board::init(int m) 
{
	scoreAdjustable = false;
	int i = 0;
	width = 15;
	srand(time(NULL));
	select1 = -1;
	select2 = -1;
	tiles.clear();
	tiles.reserve(width*17);
	while (i < width*17) 
	{
		board[i] = (rand()%7) + 1;
		tiles.emplace_back(0, 0, i % width, i / width, board[i]);
		i++;
	}
}

void M3Board::update(float displacement) 
{
	// Update the tiles.
	int id, id2, temp, goalX, goalY;
	addToScore = 0;
	moved = false;
	allStill = true;
	greatestMatchedTile = -1;
	numMatchedTiles = -1;
	// Search for any matches, then make all tiles fall, and finally move each tile.
	bool tempStill = true;
	for (std::vector<Tile>::iterator tile = tiles.begin(); tile != tiles.end(); ++tile)
	{
		id = tile->goalX + (width * tile->goalY);
		tile->update(0, board[id], false);
		tempStill = tile->still;
		if (tempStill == false) {
			allStill = false;
		}
	}
	matchSearch();
	if (numMatchedTiles > 3) {
		matchTiles();
	}

	for (std::vector<Tile>::iterator tile = tiles.begin(); tile != tiles.end(); ++tile)
	{
		id = tile->goalX + (width * tile->goalY);
		
		
		tile->update(displacement, board[id], false);
		if (board[id] != 0) 
		{
			
		}
		else 
		{
			// At the top of the board, spawn a new random tile.
			if (id >= width*16)
			{
				board[id] = (rand() % 7) + 1;
				tile->y = 1.15f;
				tile->update(0, board[id], true);
			}

			// Otherwise, swap with above tiles.
			else
			{
				if (board[id + width] == -1)
				{
					// Check if it is not the leftmost tile.
					if (id % width >= 1)
					{
						// If the tile diagonally left is a solid wall.
						if (board[id + width - 1] == -1)
						{
							// check if it is not the rightmost tile.
							if (id % width <= (width-1))
							{
								// If the tile diagonally to the right is a solid wall.
								if (board[id + width + 1] == -1)
								{
									// Leave space empty.

								}
								else
								{
									// Prevent two empty spaces above one another to continously swap with one another.
									if (board[id + width + 1] > 0) 
									{
										// Swap with above to the right.
										temp = board[id];
										board[id] = board[id + width+1];
										board[id + width+1] = temp;


										for (std::vector<Tile>::iterator tile2 = tiles.begin(); tile2 != tiles.end(); tile2++)
										{
											id2 = tile2->goalX + (width * tile2->goalY);
											if (id2 == (id + width + 1)) 
											{
												goalX = tile2->goalX; goalY = tile2->goalY;
												tile2->goalX = tile->goalX; tile2->goalY = tile->goalY;
												tile->goalX = goalX; tile->goalY = goalY;
												break;
											}
										}
									}
								}
							}
						}
						else
						{
							// Prevent two empty spaces above one another to continously swap with one another.
							if (board[id + width - 1] > 0) 
							{
								// Swap with above to the left.
								temp = board[id];
								board[id] = board[id + width - 1];
								board[id + width - 1 ] = temp;
								for (std::vector<Tile>::iterator tile2 = tiles.begin(); tile2 != tiles.end(); tile2++)
								{
									id2 = tile2->goalX + (width * tile2->goalY);
									if (id2 == (id + width - 1)) {
										goalX = tile2->goalX; goalY = tile2->goalY;
										tile2->goalX = tile->goalX; tile2->goalY = tile->goalY;
										tile->goalX = goalX; tile->goalY = goalY;
										break;
									}
								}
							}
						}
					}

				}
				else
				{
					// Prevent two empty spaces above one another to continously swap with one another.
					if (board[id + width] > 0) 
					{
						// Swap with above.
						temp = board[id];
						board[id] = board[id + width];
						board[id + width] = 0;

						for (std::vector<Tile>::iterator tile2 = tiles.begin(); tile2 != tiles.end(); tile2++)
						{
							id2 = tile2->goalX + (width * tile2->goalY);
							if (id2 == (id + width))
							{
								goalX = tile2->goalX; goalY = tile2->goalY;
								tile2->goalX = tile->goalX; tile2->goalY = tile->goalY;
								tile->goalX = goalX; tile->goalY = goalY;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void M3Board::draw(TextureManager& tex, Renderer& ren) 
{
	auto deviceContext = ren.getDeviceContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	// Draw board.
	tex.changeTex(ren, 9);
	Renderer::Vertex vertexBuffer[4] =
	{
		{-1, 1, 0, 0, 0 },
		{-1, -1, 0, 0, 1 },
		{1, 1, 0, 1, 0 },
		{1, -1, 0, 1, 1 }
	};

	// Setup vertex buffer.
	assert(mBVB != NULL);
	deviceContext->Map(mBVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(mBVB, 0);

	// Bind our vertex buffer
	stride = sizeof(Renderer::Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mBVB, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);
	

	if (select1 != -1) {
		// Draw selection box.

		tex.changeTex(ren, -1);
		Renderer::Vertex selectBuffer[4] =
		{
			{ ((select1 % width) * 0.1f) - 0.75f , ((select1 / width) * 0.1f) - 0.75f, 0, 0, 0 },
			{ ((select1 % width) * 0.1f) - 0.75f, ((select1 / width) * 0.1f) - 0.85f, 0, 0, 1 },
			{ ((select1 % width) * 0.1f) - 0.65f, ((select1 / width) * 0.1f) - 0.75f, 0, 1, 0 },
			{ ((select1 % width) * 0.1f) - 0.65f, ((select1 / width) * 0.1f) - 0.85f, 0, 1, 1 }
		};

		// Setup vertex buffer.
		assert(mSVB != NULL);
		deviceContext->Map(mSVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		memcpy(resource.pData, selectBuffer, sizeof(selectBuffer));
		deviceContext->Unmap(mSVB, 0);

		// Bind our vertex buffer
		stride = sizeof(Renderer::Vertex);
		offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &mSVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->DrawIndexed(6, 0, 0);
	}

	/*
	// Draw vertical lines
	while (x <= 0.95f) 
	{
		Renderer::Vertex vertexBuffer[4] = 
		{
			{ x - 0.001f, 0.85f, 0, 0, 0 },
			{ x - 0.001f, -0.85f, 0, 0, 1 },
			{ x + 0.001f, 0.85f, 0, 1, 0 },
			{ x + 0.001f, -0.85f, 0, 1, 1 }
		};

		// Setup vertex buffer.
		D3D11_MAPPED_SUBRESOURCE resource;
		assert(m_pVertexBuffer != NULL);
		deviceContext->Map(mBVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
		deviceContext->Unmap(mBVB, 0);

		// Bind our vertex buffer
		UINT stride = sizeof(Renderer::Vertex);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &mBVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw Box
		deviceContext->DrawIndexed(6, 0, 0);
		x += 0.1f;
	}

	// Draw horizontal lines.
	while (y <= 0.95f) 
	{
		Renderer::Vertex vertexBuffer[4] = 
		{
			{ -0.85f, y + 0.001f, 0, 0, 0 },
			{ -0.85f, y - 0.001f, 0, 0, 1 },
			{ 0.85f, y + 0.001f, 0, 1, 0 },
			{ 0.85f, y - 0.001f, 0, 1, 1 }
		};

		// Setup vertex buffer.
		D3D11_MAPPED_SUBRESOURCE resource;
		assert(m_pVertexBuffer != NULL);
		deviceContext->Map(mBVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
		deviceContext->Unmap(mBVB, 0);

		// Bind our vertex buffer
		UINT stride = sizeof(Renderer::Vertex);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &mBVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw Box
		deviceContext->DrawIndexed(6, 0, 0);
		y += 0.1f;
	}

	*/
	// Draw tiles
	tex.changeTex(ren, 8);
	for (std::vector<Tile>::iterator tile = tiles.begin(); tile != tiles.end(); tile++)
	{
		// Identify tile by where it's supposed to be, and draw it.
		float goalX = tile->goalX;
		float goalY = tile->goalY;
		int id = goalX + (width * goalY);

		Renderer::SquareObj vertexBuf = tile->update(0, board[id], false);
		auto deviceContext = ren.getDeviceContext();
		Renderer::Vertex vertexBuffer[4] = 
		{
			vertexBuf.vertices[0],
			vertexBuf.vertices[1],
			vertexBuf.vertices[2],
			vertexBuf.vertices[3],
		};

		// Update vertex and pixel buffers
		D3D11_MAPPED_SUBRESOURCE resource;
		deviceContext->Map(mTVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
		deviceContext->Unmap(mTVB, 0);

		// Bind our Player shaders
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind our vertex buffer
		UINT stride = sizeof(Renderer::Vertex);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &mTVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw
		deviceContext->DrawIndexed(6, 0, 0);
		
	}

}

void M3Board::matchSearch()
{
	int vertMatch;
	int horMatch;
	unsigned int tempScore = 0;
	int id;

	if (allStill) 
	{
		for (std::vector<Tile>::iterator tile = tiles.begin(); tile != tiles.end(); tile++)
		{
			id = (tile->goalX) + (width * tile->goalY);
			vertMatch = 1;
			horMatch = 1;

			if (board[id] >= 1) 
			{
				// Below point of switch
				if (id >= width)
				{
					if (board[id - width] == board[id])
					{
						vertMatch++;
					
						if (id >= (2 * width))
						{
							if (board[id - (2 * width)] == board[id])
							{
							
								vertMatch++;

							}
						}
					}
				}

				// Above point of switch
				if (id <= width * 16) {
					if (board[id + width] == board[id]) {
					
						vertMatch++;
						if (id <= width * 15) {
							if (board[id + (2 * width)] == board[id]) {
							
								vertMatch++;
							}
						}
					}
				}

				// Left side
				if (id % width >= 1) {
					if (board[id - 1] == board[id]) {
						
						horMatch++;
						if (id % width >= 2) {
							if (board[id - 2] == board[id]) {
								
								horMatch++;
							}

						}
					}
				}

				// Right side
				if (id % width <= (width - 1)) {
					if (board[id + 1] == board[id]) {
						
						horMatch++;
						if (id % width <= (width - 2)) {
							if (board[id + 2] == board[id]) {
								
								horMatch++;
							}

						}
					}
				}

				// Select the largest cluster of matched tiles first.
				if ((vertMatch >= 3) || (horMatch >= 3))
				{
					if (vertMatch + horMatch > numMatchedTiles) {
						numMatchedTiles = vertMatch + horMatch;
						greatestMatchedTile = id;
					}
				}
			}
		}
	}
}

// Update the largest cluster of matched tiles.
void M3Board::matchTiles()
{
	if (allStill) {
		int vertMatch=1;
		int horMatch=1;
		int idx[9];
		int id;
		memset(idx, 5, sizeof(int) * 9);
		for (std::vector<Tile>::iterator tile = tiles.begin(); tile != tiles.end(); tile++)
		{
			id = (tile->goalX) + (width * tile->goalY);
			if (id == greatestMatchedTile) 
			{
				// Below point of switch
				if (id >= width)
				{
					if (board[id - width] == board[id])
					{
						vertMatch++;
						idx[5] = 1;
						if (id >= (2 * width))
						{
							if (board[id - (2 * width)] == board[id])
							{
								idx[6] = 1;
								vertMatch++;

							}
						}
					}
				}

				// Above point of switch
				if (id <= width * 16) {
					if (board[id + width] == board[id]) {
						idx[3] = 1;
						vertMatch++;
						if (id <= width * 15) {
							if (board[id + (2 * width)] == board[id]) {
								idx[2] = 1;
								vertMatch++;
							}
						}
					}
				}

				// Left side
				if (id % width >= 1) {
					if (board[id - 1] == board[id]) {
						idx[1] = 1;
						horMatch++;
						if (id % width >= 2) {
							if (board[id - 2] == board[id]) {
								idx[0] = 1;
								horMatch++;
							}

						}
					}
				}

				// Right side
				if (id % width <= (width - 1)) 
				{
					if (board[id + 1] == board[id]) {
						idx[7] = 1;
						horMatch++;
						if (id % width <= (width - 2)) {
							if (board[id + 2] == board[id]) {
								idx[8] = 1;
								horMatch++;
							}

						}
					}
				}
					
				if (board[id] == 7)
				{
					addToScore = -1;
					if (horMatch >= 3) {
						if (idx[7] == 1) {
							board[id + 1] = 0;
							addToScore += (2 * addToScore);
						}
						if (idx[8] == 1) {
							board[id + 2] = 0;
							addToScore += (2 * addToScore);
						}
						if (idx[0] == 1) {
							board[id - 2] = 0;
							addToScore += (2 * addToScore);
						}
						if (idx[1] == 1) {
							board[id - 1] = 0;
							addToScore += (2 * addToScore);
						}
					}
				}

				else
				{
					if (scoreAdjustable)
					{
						addToScore = 1;
						if (horMatch >= 3) {
							if (idx[7] == 1) {
								board[id + 1] = 0;
								addToScore += (2 * addToScore);
							}
							if (idx[8] == 1) {
								board[id + 2] = 0;
								addToScore += (2 * addToScore);
							}
							if (idx[0] == 1) {
								board[id - 2] = 0;
								addToScore += (2 * addToScore);
							}
							if (idx[1] == 1) {
								board[id - 1] = 0;
								addToScore += (2 * addToScore);
							}
						}
					}
				}

				if (vertMatch >= 3) {
					if (idx[2] == 1) {
						board[id + (2 * width)] = 0;
						addToScore += (2 * addToScore);
					}
					if (idx[3] == 1) {
						board[id + width] = 0;
						addToScore += (2 * addToScore);
					}
					if (idx[5] == 1) {
						board[id - width] = 0;
						addToScore += (2 * addToScore);
					}
					if (idx[6] == 1) {
						board[id - (2 * width)] = 0;
						addToScore += (2 * addToScore);
					}
				}
				

				board[id] = 0;
				break;
			}
		}
	}
}

void M3Board::getMouseLoc(float mx, float my) {
	// Check if mouse is inside the grid.
	if ((mx > -0.75f) & (mx < 0.75f) & (my > -0.85f) & (my < 0.85f)) {
		// Translate mouse position into index. Not completely accurate yet.
		int id, id2, val;
		float temp;
		int xOff, yOff;
		xOff = -1; yOff = -1;
		xOff = fabs((mx + 0.75f) / 0.1f);
		yOff = fabs((my + 0.85f) / 0.1f);
	
		id = xOff + (width * yOff);

		// Update the selection. Perform switch if two valid locations are selected. Can't move spaces.
		if (select1 >= 0) {
			select2 = xOff + (yOff * width);
			if (select2 != select1) 
			{
				if (board[select2] > 0)
				{
					for (std::vector<Tile>::iterator tile = tiles.begin(); tile != tiles.end();)
					{
						id = tile->goalX + (width * tile->goalY);
						if (id == select1) 
						{
							for (std::vector<Tile>::iterator tile2 = tiles.begin(); tile2 != tiles.end();)
							{
								id2 = tile2->goalX + (width * tile2->goalY);
								if (id2 == select2) {
									temp = tile->goalX;
									tile->goalX = tile2->goalX;
									tile2->goalX = temp;
									temp = tile->goalY;
									tile->goalY = tile2->goalY;
									tile2->goalY = temp;
									break;
								}
								tile2++;

							}
							break;
						}

						tile++;

					}
					moved = true;
					scoreAdjustable = true;
					val = board[select1];
					board[select1] = board[select2];
					board[select2] = val;
					select1 = -1;
					select2 = -1;

				}
				else
				{
					select1 = -1;
					select2 = -1;
					
				}
			}
		}
		else 
		{
			select1 = xOff + (yOff * width);
			if (board[select1] <= 0) {
				select1 = -1;
				select2 = -1;
			}
			
		}
	}
	// Reset selection if clicked outside boundries of the board.
	else {
		select1 = -1;
		select2 = -1;
	}
}

void M3Board::createBoardMesh(Renderer& ren)
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

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mBVB);
	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mSVB);

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

void M3Board::createTileMesh(Renderer& ren)
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

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mTVB);
}