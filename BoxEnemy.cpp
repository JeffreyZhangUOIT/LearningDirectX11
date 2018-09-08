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
#include "BoxEnemy.h"

BoxEnemy::BoxEnemy(float posx, float posy) {
	x = posx;
	y = posy;
	EnemyHp = 3;
	touch = false;
	slope = 0;

}

BoxEnemy::BoxEnemy(const BoxEnemy &b2) {
	x = b2.x;
	y = b2.y;
	EnemyHp = b2.EnemyHp;
	touch = b2.touch;
	slope = b2.slope;
}

bool BoxEnemy::bulletCollision(float* pos)
{
	if ((fabs(y - pos[1]) < 0.05f) && (fabs(x - pos[0]) < 0.05f))
	{
		EnemyHp--;
		return true;
	}

	return false;
}
BoxEnemy::~BoxEnemy() {

}
float* BoxEnemy::getPos()
{
	point[0] = x;
	point[1] = y;
	return point;
}
BoxEnemy::ArrVer BoxEnemy::update(float* playerPos, float displacement) {
	float hpPercent = (EnemyHp / 5.0f);
	slope = atan2((playerPos[0] - x), (playerPos[1] - y));

	if (fabs(x - playerPos[0]) <= 1.3f){
		if ((x - playerPos[0]) > 0) {
			x -= (0.45f * displacement);
		}
		if ((x - playerPos[0]) < 0) {
			x += (0.45f * displacement);
		}
	}
	else {
		if ((x - playerPos[0]) > 0) {
			x += (0.45f * displacement);
		}
		if ((x - playerPos[0]) < 0) {
			x -= (0.45f * displacement);
		}
	}
	if (fabs(y - playerPos[1]) <= 1.3f) {
		if ((y - playerPos[1]) > 0) {
			y -= (0.5f * displacement);
		}
		if ((y - playerPos[1]) < 0) {
			y += (0.5f * displacement);
		}
	}
	else {
		if ((y - playerPos[1]) > 0) {
			y += (0.5f * displacement);
		}
		if ((y - playerPos[1]) < 0) {
			y -= (0.5f * displacement);
		}
	}

	if (x < -1.05f) {
		x = 1.05f;
	}
	if (x > 1.05f) {
		x = -1.05f;
	}
	if (y > 1.05f) {
		y = -1.05f;
	}
	if (y < -1.05f) {
		y = 1.05f;
	}

	float distance = sqrt(((x - playerPos[0])*(x - playerPos[0])) + ((y - playerPos[1])*(y - playerPos[1])));
	if (distance <= 0.1f) 
	{
		touch = true;
	}
	else 
	{
		touch = false;
	}
	
	DirectX::XMFLOAT4 p1, p2, p3, p4;
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMVECTOR translate = DirectX::XMVectorSet(x, y, 0, 0);
	DirectX::XMVECTOR RotationQuaternion = DirectX::XMQuaternionRotationAxis({ 0, 0, -1, 0 }, slope);
	DirectX::XMVECTOR RotationCenter = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMMATRIX transform = DirectX::XMMatrixAffineTransformation(scale, RotationCenter, RotationQuaternion, translate);

	DirectX::XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, transform);
	DirectX::XMVECTOR v1 = DirectX::XMVectorSet(-0.05f, -0.05f, 0, 1);
	DirectX::XMVECTOR v2 = DirectX::XMVectorSet(0.05f, -0.05f, 0, 1);
	DirectX::XMVECTOR v3 = DirectX::XMVectorSet(-0.05f, 0.05f, 0, 1);
	DirectX::XMVECTOR v4 = DirectX::XMVectorSet(0.05f, 0.05f, 0, 1);

	v1 = DirectX::XMVector4Transform(v1, transform);
	v2 = DirectX::XMVector4Transform(v2, transform);
	v3 = DirectX::XMVector4Transform(v3, transform);
	v4 = DirectX::XMVector4Transform(v4, transform);

	DirectX::XMStoreFloat4(&p1, v1);
	DirectX::XMStoreFloat4(&p2, v2);
	DirectX::XMStoreFloat4(&p3, v3);
	DirectX::XMStoreFloat4(&p4, v4);

	ArrVer newVertices;
	newVertices.vertices[0] = { p1.x, p1.y, 0, 0, 0 };
	newVertices.vertices[1] = { p2.x, p2.y, 0, 1, 0 };
	newVertices.vertices[2] = { p3.x, p3.y, 0, 0, 1 };
	newVertices.vertices[3] = { p4.x, p4.y, 0, 1, 1 };
	newVertices.vertices[4] = { x - 0.05f, y + 0.06f, 0, 0, 0 };
	newVertices.vertices[5] = { ((x - 0.05f) + (0.17f * hpPercent)), y + 0.06f, 0, 1, 0 };
	newVertices.vertices[6] = { x - 0.05f, y + 0.08f, 0, 0, 1 };
	newVertices.vertices[7] = { ((x - 0.05f) + (0.17f * hpPercent)), y + 0.08f, 0, 1, 1 };

	return newVertices;
}

bool BoxEnemy::checkCollision(){
	return touch;
}
