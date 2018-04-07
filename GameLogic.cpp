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

void GameLogic::updateTDS( Renderer& ren, float* pos) {

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
			it = boxes.erase(it);
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
	if ((unsigned)boxes.size() < (unsigned)(difficulty * 4)) {
		if (((fabs(x - pos[0]) > 0.3f) || (fabs(x - pos[0]) > 0.3f))) {
			spawnBoxEnemy(x, y, ren);
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

	BoxEnemy::Vertex vertcies[4] = {
		vertices.vertices[0],
		vertices.vertices[1],
		vertices.vertices[2],
		vertices.vertices[3],
	};

	D3D11_MAPPED_SUBRESOURCE resource;
	assert(m_pVertexBuffer != NULL);
	deviceContext->Map(m_pBoxEnemyVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertcies, sizeof(vertcies));
	deviceContext->Unmap(m_pBoxEnemyVertexBuffer, 0);

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(BoxEnemy::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pBoxEnemyVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pBoxEnemyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}