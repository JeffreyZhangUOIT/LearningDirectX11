/*
Copyright 2017, 2018 Jeffrey Zhang

This file is part of ProjectFiasco.

ProjectFiasco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ProjectFiasco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ProjectFiasco.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "GameLogic.h"

GameLogic::GameLogic(Renderer& ren) {
	game = 0;
	player.createMesh(ren);
	raceMap.createMesh(ren);
	createMeshofBullets(ren);
	createMeshofBoxEnemies(ren);
	createMeshForProgress(ren);
	createMeshofPacEntites(ren);
	board.createBoardMesh(ren);
	board.createTileMesh(ren);
	pacWalker.createMesh(ren);
	pacTiles.initBG1(ren);
}

GameLogic::~GameLogic() {
	collection.clear();
	boxes.clear();
}


void GameLogic::update(TextHandler& text, Renderer& ren, GameView& pv, Timer& time, TextureManager& tex, MinigameManager& miniMan) {

	currentFT = time.deltaFT();
	deltaFT = currentFT - prevFT;
	prevFT = time.deltaFT();
	dataObj.minimeta.elapsedTime += deltaFT;
	timeSinceLastSpawn += deltaFT;
	delay -= deltaFT;
	enemyConductor(ren, pv.getPos(), time);

	// Update for a top-down shooter minigame
	if (game == 1) {
		progress = dataObj.minimeta.kills /(float) (10 * dataObj.minimeta.difficulty);
		// Raw code for spawning bullets
		if (((GetKeyState(VK_LBUTTON) & 0x80) != 0) & (delay <= 0)) {
			collection.emplace_back(pv.getPos(), ren, time);
			delay = 0.02f;
		}

		// For each bullet, update and draw it using texture 2
		tex.changeTex(ren, 11);
		for (std::vector<Bullet>::iterator bul = collection.begin(); bul != collection.end();)
		{
			drawBullets(ren, bul->update(deltaFT));

			bool outOfBounds = false;
			outOfBounds = bul->OOB;

			if (outOfBounds)
			{
				bul = collection.erase(bul);
			}
			else
			{
				bul++;
			}

		}

		// For each box enemy, update and draw it. Enemies determine their own texture.
		for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end();)
		{
			drawBoxEnemies(tex, ren, it->update(pv.getPos(), deltaFT));
			for (std::vector<Bullet>::iterator bul = collection.begin(); bul != collection.end();)
			{
				float * bPos = bul->getPos();

				if (it->bulletCollision(bPos))
				{
					bul = collection.erase(bul);
				}

				else
				{
					bul++;
				}
			}
			if (it->EnemyHp <= 0) 
			{
				float * someboxPos = it->getPos();
				DirectX::XMVECTORF32 white = { 1, 1, 1, 1 };
				text.deathRattle(someboxPos[0], someboxPos[1], white);
				dataObj.minimeta.score += 1;
				dataObj.minimeta.kills += 1;
				it = boxes.erase(it);
			}
			else 
			{
				it++;
			}
		}

		// Update and draw the player
		if (playerDamage()) {
			pv.setPos(player.update(ren, tex, pv.getPos(), true, game));
			dataObj.minimeta.playerHp -= 1;
		}
		else
		{
			pv.setPos(player.update(ren, tex, pv.getPos(), false, game));
		}
		player.draw(ren);
		// Player died, determine which scene to go to and go.
	}

	// Update for match 3 minigame
	if (game == 2) {
		progress = (float) dataObj.minimeta.score / (float)1500;

		// Raw code for spawning bullets
		if (((GetKeyState(VK_LBUTTON) & 0x80) != 0) & (delay <= 0)) {
			float* mousePos = pv.getAim();

			board.getMouseLoc(mousePos[0], mousePos[1]);
			delay = 0.05f;
		}

		// Send board data to minigame meta data object
		scoreBuffer += board.addToScore;
		if (scoreBuffer > 0)
		{
			scoreBuffer -= 1;
			dataObj.minimeta.score += 1;
		}
		if (scoreBuffer < 0)
		{
			scoreBuffer += 1;
			dataObj.minimeta.score -= 1;
		}
		if (board.moved == true) {
			dataObj.minimeta.moves += 1;
			if (dataObj.minimeta.moves == (60 / dataObj.minimeta.difficulty))
			{
				dataObj.minimeta.score += scoreBuffer;
				scoreBuffer = 0;
			}
		}

		// Update and draw a new board.
		board.update(deltaFT);
		board.draw(tex, ren);

	}

	// Update for driving minigame
	if (game == 3) {
		progress = (float) dataObj.minimeta.distanceTraveled / (float)(dataObj.minimeta.difficulty * 50);
		float * tempPos = player.update(ren, tex, pv.getPos(), playerDamage(), game);
		acceleration = (1 / (1 + (1.0f - tempPos[1])));
		float distance = acceleration * deltaFT * 10;
		raceMap.update(distance);
		raceMap.draw(ren, tex);
		dataObj.minimeta.distanceTraveled += distance;

		// For each obstacle, update and draw it using texture 2
		tex.changeTex(ren, 10);
		for (std::vector<Obstacles>::iterator bul = automobiles.begin(); bul != automobiles.end();)
		{
			drawCars(ren, bul->update(pv.getPos(), distance));

			if (bul->OOB)
			{
				bul = automobiles.erase(bul);
			}
			else
			{
				bul++;
			}
		}

		// Update and draw the player
		if (playerDamage()) {
			pv.setPos(player.update(ren, tex, pv.getPos(), true, game));
			dataObj.minimeta.playerHp -= 1;
		}
		else
		{
			pv.setPos(player.update(ren, tex, pv.getPos(), false, game));
		}

		player.draw(ren);

	}

	// Update for pacman clone
	if (game == 4)
	{
		progress = dataObj.minimeta.captures / (float)(dataObj.minimeta.difficulty * 4);
		pacWalker.update(pacTiles, deltaFT);
		pacWalker.draw(ren, tex);
		//pacTiles.drawBoard(ren, tex); Debug tile placement.

		for (std::vector<PacmanEntites>::iterator pac = pacEntities.begin(); pac != pacEntities.end(); )
		{
			tex.changeTex(ren, 16);
			drawPacEntities(ren, pac->update(pacTiles, pacWalker, deltaFT));
			if ((fabs(pac->x - pacWalker.x) < 0.12f) & (fabs(pac->y - pacWalker.y) < 0.12f))
			{
				if (pac->type == 2)
				{
					dataObj.minimeta.captures += 1;
				}
				else
				{
					if (pac->type == 0)
					{
						pacWalker.modSpeed(true);
					}
					else
					{
						pacWalker.modSpeed(false);
					}	
				}
				pac = pacEntities.erase(pac);
			}
			else 
			{
				pac++;
			}
		}
	}
	drawProgressBar(tex, ren, progress);
}

void GameLogic::init(Renderer& ren, Timer& timer) {

	dataObj.minimeta = { 3, 5, 0, 60000, 0, 0, 0, 0, 0, 0, 0 };
	dataObj.minimeta.elapsedTime = 0;
	dataObj.minimeta.score = 0;
	dataObj.minimeta.kills = 0;
	dataObj.minimeta.moves = 0;
	dataObj.minimeta.captures = 0;
	if (game == 2) 
	{	
		dataObj.minimeta.timeLimit = 300;
	}
	if (game == 3) 
	{
		dataObj.minimeta.timeLimit = 300;
	}

	if (game == 4) 
	{
		dataObj.minimeta.timeLimit = 120;
		int students = dataObj.minimeta.difficulty * 4;
		float x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))) - 1.0f;
		float y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))) - 1.0f;
		while (students > 0)
		{
			if (((x > 0.2f) || (x < -0.2f)) & ((y > 0.2f) || (y < -0.2f)))
			{
				if (pacTiles.checkSquareArea(x, y) == true)
				{
					pacEntities.emplace_back(2, x, y);
					students--;
				}
			}
			x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))) - 1.0f;
			y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))) - 1.0f;
		}
	}
	boxes.clear();
	player.init();
	board.init(20);
	pacWalker.init();
	collection.clear();
	automobiles.clear();
	acceleration = 1;
	srand(time(NULL));
	timer.resetTime();
	currentFT = timer.deltaFT();
	prevFT = timer.deltaFT();
	scoreBuffer = 0;
}

// Deletes enemies that have collided with the player, and reports the collision
bool GameLogic::playerDamage() {
	
	// Delete box enemies if they collide with player in top down shooter.
	if (game == 1) {
		for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end();)
		{
			if (it->checkCollision()) {
				boxes.erase(it);
				return true;
			}
			else
			{
				it++;
			}
		}
	}
	// Delete cars if they collide with player in driving minigame.
	if (game == 3) {
		for (std::vector<Obstacles>::iterator car = automobiles.begin(); car != automobiles.end();)
		{
			if (car->checkCollision()) {
				automobiles.erase(car);
				return true;
			}
			else 
			{
				car++;
			}
		}
	}
	return false;
}

// Spawns obstacles/opponents
void GameLogic::enemyConductor(Renderer& ren, float* pos, Timer& time) {
	float x, y;

	// Top down shooter
	if (game == 1) {
		if (timeSinceLastSpawn >= (std::pow(3, (-0.1f * (dataObj.minimeta.elapsedTime - 10)) - (dataObj.minimeta.difficulty / 10.0f))) ) {
			if (boxes.size() < 200) {
				x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.6f))) - 1.3f;
				y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.6f))) - 1.3f;

				/*
				Ensure the following:
				A. The enemy spawns off screen.
				B. The enemy does not spawn near the player.
				C. The enemy does not spawn on the opposite size of the player, allowing it to warp behind the player.
				D. Doesn't stop distant enemies from walking around, you should be able to see them coming.
				*/

				if (((fabs(y) > 1.05f) || (fabs(x) > 1.05f)) & ((fabs(x - pos[0])) > 0.4f) & ((fabs(y - pos[1])) > 0.4f) & ((fabs(x - pos[0]) < 1.9f) || (fabs(y - pos[1]) < 1.9f)))
				{
					timeSinceLastSpawn = 0;
					boxes.emplace_back(x, y);
				}
			}
		}
	}
	
	// Driving minigame
	if (game == 3) {
		if (timeSinceLastSpawn >= (std::pow(1.5f, (-0.05f * (dataObj.minimeta.distanceTraveled - 50)) - (dataObj.minimeta.difficulty / 10.0f)))) {
			// Spawn other drivers.
			x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.2f))) - 0.6f;
			int dir;
			if (((x < -0.16f) & (x > -0.46f)) || x < -0.51f){
				dir = -1;
				automobiles.emplace_back(x, 1.1f, dir);
			}
			else if (((x>-0.11f) & (x < 0.10f)) || ((x>0.15f) & (x<0.35f)) || x>0.4f) {
				dir = 1;
				automobiles.emplace_back(x, -1.1f, dir);
			}
			timeSinceLastSpawn = 0;
		}
	}

	// Pacman minigame
	if (game == 4)
	{
		if (timeSinceLastSpawn >= dataObj.minimeta.difficulty * 30)
		{
			bool notPlaced = true;
			int powerup = rand() % 2;
			while (notPlaced)
			{
				x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))) - 1.0f;
				y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))) - 1.0f;

				if (pacTiles.checkSquareArea(x, y) == true)
				{
					
					if (powerup != 0)
					{
						pacEntities.emplace_back(1, x, y);
						timeSinceLastSpawn = 0;
					}
					else
					{
						pacEntities.emplace_back(0, x, y);
						timeSinceLastSpawn = 0;
					}
					notPlaced = false;
				}
			}
		}
	}
	
}

void GameLogic::drawCars(Renderer& renderer, Obstacles::ArrVer vertices) {
	auto deviceContext = renderer.getDeviceContext();
	Renderer::Vertex vertexBuffer[4] = {
		vertices.vertices[0],
		vertices.vertices[1],
		vertices.vertices[2],
		vertices.vertices[3],
	};

	// Update vertex and pixel buffers
	D3D11_MAPPED_SUBRESOURCE resource;
	deviceContext->Map(m_pBulletVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(m_pBulletVertexBuffer, 0);

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pBulletVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBulletIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}

void GameLogic::createMeshofBullets(Renderer& ren)
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

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pBulletVertexBuffer);

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
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &m_pBulletIndexBuffer);
	ren.getDeviceContext()->IASetIndexBuffer(m_pBulletIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void GameLogic::drawBullets(Renderer& renderer, Renderer::SquareObj vertices) {

	auto deviceContext = renderer.getDeviceContext();
	Renderer::Vertex vertexBuffer[4] = {
		vertices.vertices[0],
		vertices.vertices[1],
		vertices.vertices[2],
		vertices.vertices[3],
	};

	

	// Update vertex and pixel buffers
	D3D11_MAPPED_SUBRESOURCE resource;
	deviceContext->Map(m_pBulletVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(m_pBulletVertexBuffer, 0);

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pBulletVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBulletIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
	
}

void GameLogic::createMeshofBoxEnemies(Renderer& ren)
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
	vertexData.pSysMem = m_pVertices;

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pBoxEnemyVertexBuffer);

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
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &m_pBoxEnemyIndexBuffer);
	ren.getDeviceContext()->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void GameLogic::createMeshForProgress(Renderer& ren)
{
	// Create our vertext buffer
	Renderer::Vertex vertices[4] =
	{
		{ 0.8f, 1, 0, 0, 0 },
		{ 0.8f, 0.95f, 0, 0, 1 },
		{ 1, 1, 0, 1, 0 },
		{ 1, 0.95f, 0, 1, 1 }
	};

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = m_pVertices;

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pBoxEnemyVertexBuffer);

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
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &m_pBoxEnemyIndexBuffer);
	ren.getDeviceContext()->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void GameLogic::drawProgressBar(TextureManager& tex, Renderer& renderer, float prog)
{
	auto deviceContext = renderer.getDeviceContext();
	
	Renderer::Vertex vertexBuffer[4] = {
		{ 0.6f, 0.95f, 0, 0, 0 },
		{ 0.6f, 0.9f, 0, 0, 1 },
		{ 1, 0.95f, 0, 1, 0 },
		{ 1, 0.90f, 0, 1, 1 }
	};

	tex.changeTex(renderer, 20);

	// Setup vertex buffer.
	D3D11_MAPPED_SUBRESOURCE resource;
	assert(m_pVertexBuffer != NULL);
	deviceContext->Map(m_pBoxEnemyVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(m_pBoxEnemyVertexBuffer, 0);

	// Setup topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pBoxEnemyVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw Box

	deviceContext->DrawIndexed(6, 0, 0);

	// Draw progress icon.
	Renderer::Vertex hpBackBuffer[4] = {
		{ 0.78f + (prog * 0.2f), 0.95f, 0, 0, 0 },
		{ 0.78f + (prog * 0.2f), 0.9f, 0, 0, 1 },
		{ 0.8f + (prog * 0.2f), 0.95f, 0, 1, 0 },
		{ 0.8f + (prog * 0.2f), 0.9f, 0, 1, 1 }
	};
	tex.changeTex(renderer, 21);

	deviceContext->Map(m_pBoxEnemyVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, hpBackBuffer, sizeof(hpBackBuffer));
	deviceContext->Unmap(m_pBoxEnemyVertexBuffer, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0, 1, &m_pBoxEnemyVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);
}

void GameLogic::drawBoxEnemies(TextureManager& tex, Renderer& renderer, BoxEnemy::ArrVer vertices) {
	auto deviceContext = renderer.getDeviceContext();

	Renderer::Vertex vertexBuffer[4] = {
		vertices.vertices[0],
		vertices.vertices[1],
		vertices.vertices[2],
		vertices.vertices[3],
	};

	tex.changeTex(renderer, -1);

	// Setup vertex buffer.
	D3D11_MAPPED_SUBRESOURCE resource;
	assert(m_pVertexBuffer != NULL);
	deviceContext->Map(m_pBoxEnemyVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(m_pBoxEnemyVertexBuffer, 0);

	// Setup topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pBoxEnemyVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw Box
	
	deviceContext->DrawIndexed(6, 0, 0);

	// Draw red hp bar background.
	Renderer::Vertex hpBackBuffer[4] = {
		vertices.vertices[4],
		vertices.vertices[5],
		vertices.vertices[6],
		vertices.vertices[7],
	};
	tex.changeTex(renderer, 3);

	deviceContext->Map(m_pBoxEnemyVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, hpBackBuffer, sizeof(hpBackBuffer));
	deviceContext->Unmap(m_pBoxEnemyVertexBuffer, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0, 1, &m_pBoxEnemyVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);

}

void GameLogic::createMeshofPacEntites(Renderer& ren)
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

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pPacEntitiesVertexBuffer);

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
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &m_pPacEntitiesIndexBuffer);
	ren.getDeviceContext()->IASetIndexBuffer(m_pPacEntitiesIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void GameLogic::drawPacEntities(Renderer& renderer, Renderer::SquareObj vertices) {

	auto deviceContext = renderer.getDeviceContext();
	
	Renderer::Vertex vertexBuffer[4] = {
		vertices.vertices[0],
		vertices.vertices[1],
		vertices.vertices[2],
		vertices.vertices[3],
	};

	// Update vertex and pixel buffers
	D3D11_MAPPED_SUBRESOURCE resource;
	deviceContext->Map(m_pPacEntitiesVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(m_pPacEntitiesVertexBuffer, 0);

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pPacEntitiesVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pPacEntitiesIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);

}