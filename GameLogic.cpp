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
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "GameLogic.h"

GameLogic::GameLogic(Renderer& ren) {
	initTDS(ren);
	createMeshofBullets(ren);
	createMeshofBoxEnemies(ren);
}

GameLogic::~GameLogic() {
	collection.clear();
	boxes.clear();
}

void GameLogic::spawnBullet(bool mDown, float* pos, float* aim,  Renderer& ren)
{
	if (((GetKeyState(VK_LBUTTON) & 0x80) != 0) & (delay <= 0))
	{
		collection.emplace_back(pos, ren);
		delay = 3;
		
	}

}

void GameLogic::updateTDS(TextHandler& text, Renderer& ren, float* pos) {
	spawnDelay++;
	enemyConductor(ren, pos);
	if (delay > 0) {
		delay--;
	}

	for (std::vector<Bullet>::iterator bul = collection.begin(); bul != collection.end();)
	{
		// For each bullet, update and draw it.
		drawBullets(ren, bul->update());

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

	for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end();)
	{
		// For each box enemy, update and draw it.
		drawBoxEnemies(ren, it->update(pos));
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

		if (it->EnemyHp <= 0) {
			float * someboxPos = it->getPos();
			DirectX::XMVECTORF32 white = { 1, 1, 1, 1 };

			text.deathRattle(someboxPos[0], someboxPos[1], white);
			it = boxes.erase(it);
			deceasedEnemies++;
		}
		else {
			it++;
		}
	}
}


void GameLogic::spawnBoxEnemy(float x, float y, Renderer& ren) {
	boxes.emplace_back(x, y, ren);
	
}

void GameLogic::initTDS(Renderer& ren) {
	boxes.clear();
	collection.clear();
	deceasedEnemies = 0;

	srand(static_cast <unsigned> (time(0)));

}

bool GameLogic::playerDamage() {
	
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
	return false;
}

void GameLogic::enemyConductor(Renderer& ren, float* pos) {
	float x, y;
	x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.6f))) - 1.3f;
	y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.6f))) - 1.3f;
	timeScaledDifficulty = (30 - difficulty) - (deceasedEnemies / 2);
	if (spawnDelay >= (4 - difficulty) * timeScaledDifficulty) {
		if ((unsigned)boxes.size() < (unsigned)200) {
			if (((fabs(y) > 1) || (fabs(x) > 1))) {
				spawnDelay = 0;
				spawnBoxEnemy(x, y, ren);
			}
		}
	}
	
}

void GameLogic::createMeshofBullets(Renderer& ren)
{
	Bullet::Vertex m_pVertices[4] = {
		{ 1, 0, 0, 1, 1, 1, 1 },
		{ 0, 1, 0, 1, 1, 1, 1 },
		{ 0, 0, 0, 1, 1, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1 }
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

void GameLogic::drawBullets(Renderer& renderer, Bullet::ArrVer vertices) {
	auto deviceContext = renderer.getDeviceContext();

	Bullet::Vertex vertcies[4] = {
		vertices.vertices[0],
		vertices.vertices[1],
		vertices.vertices[2],
		vertices.vertices[3],
	};

	// Update vertex and pixel buffers
	D3D11_MAPPED_SUBRESOURCE resource;
	deviceContext->Map(m_pBulletVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertcies, sizeof(vertcies));
	deviceContext->Unmap(m_pBulletVertexBuffer, 0);

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Bullet::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pBulletVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBulletIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}

void GameLogic::createMeshofBoxEnemies(Renderer& ren)
{
	// Create our vertext buffer
	BoxEnemy::Vertex vertices[] = {
		{ 0, 0, 0, 1, 1, 1, 1 },
	{ 0, 1, 0, 1, 1, 1, 1 },
	{ 1, 0, 0, 1, 1, 1, 1 },
	{ 1, 1, 0, 1, 1, 1, 1 }
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

void GameLogic::drawBoxEnemies(Renderer& renderer, BoxEnemy::ArrVer vertices) {
	auto deviceContext = renderer.getDeviceContext();

	BoxEnemy::Vertex vertexBuffer[4] = {
		vertices.vertices[0],
		vertices.vertices[1],
		vertices.vertices[2],
		vertices.vertices[3],
	};

	// Setup vertex buffer.
	D3D11_MAPPED_SUBRESOURCE resource;
	assert(m_pVertexBuffer != NULL);
	deviceContext->Map(m_pBoxEnemyVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertexBuffer, sizeof(vertexBuffer));
	deviceContext->Unmap(m_pBoxEnemyVertexBuffer, 0);

	// Setup topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(BoxEnemy::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pBoxEnemyVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw Box
	deviceContext->DrawIndexed(6, 0, 0);

	// Draw red hp bar background.
	BoxEnemy::Vertex hpBackBuffer[4] = {
		vertices.vertices[4],
		vertices.vertices[5],
		vertices.vertices[6],
		vertices.vertices[7],
	};

	deviceContext->Map(m_pBoxEnemyVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, hpBackBuffer, sizeof(hpBackBuffer));
	deviceContext->Unmap(m_pBoxEnemyVertexBuffer, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0, 1, &m_pBoxEnemyVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(6, 0, 0);







}