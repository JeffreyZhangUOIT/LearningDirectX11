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
#include "Bullet.h"

Bullet::Bullet(float* pos, Renderer& ren, Timer& time) 
{
	x = pos[0];
	y = pos[1];
	point = new float[2];
	point[0] = x;
	point[1] = y;
	dir = pos[2];

	// Move the bullet out of the player, without drawing it. Helps make it seem like it's coming out of the right place.
	update(0.05f);
}

Bullet::Bullet(const Bullet &bul) {

	x = bul.x;
	y = bul.y;
	dir = bul.dir;
	point = nullptr;
	point =  new float[2];
	point[0] = x;
	point[1] = y;
}

Bullet::~Bullet() 
{

}

Bullet::ArrVer Bullet::update(float displacement)
{
	// Weird teleporting behavior.  (0.9f * deltaTime)
	OOB = false;
	
	if ((dir > 0) & (dir < 0.5f* 3.14f)) {
		y += (0.6f * displacement) * cosf(fabs(dir));
	}
	if (dir > 0.5f*3.14) {
		y -= (0.6f * displacement) * -cosf(fabs(dir));
	}
	if ((dir < 0) & (dir > -0.5f*3.14)) {
		y += (0.6f * displacement) * cosf(fabs(dir));
	}
	if (dir < -0.5f*3.14) {
		y -= (0.6f * displacement) * -cosf(fabs(dir));
	}
	if ((dir > 0) & (dir < 0.5f* 3.14f)) {
		x += (0.6f * displacement) * sinf(fabs(dir));
	}
	if (dir > 0.5f*3.14) {
		x += (0.6f * displacement) * sinf(fabs(dir));
	}
	if ((dir < 0) & (dir > -0.5f*3.14)) {
		x -= (0.6f * displacement) * sinf(fabs(dir));
	}
	if (dir < -0.5f*3.14) {
		x -= (0.6f * displacement) * sinf(fabs(dir));
	}
	if (dir == 0) {
		y += 0.03f;
	}
	if (fabs(x) > 1.2f || fabs(y) > 1.2f) {
		OOB = true;
	}

	

	DirectX::XMFLOAT4 p1, p2, p3, p4;
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMVECTOR translate = DirectX::XMVectorSet(x, y, 0, 0);
	DirectX::XMVECTOR RotationQuaternion = DirectX::XMQuaternionRotationAxis({ 0, 0, -1, 0 }, dir);
	DirectX::XMVECTOR RotationCenter = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMMATRIX transform = DirectX::XMMatrixAffineTransformation(scale, RotationCenter, RotationQuaternion, translate);

	DirectX::XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, transform);
	DirectX::XMVECTOR v1 = DirectX::XMVectorSet(-0.01f, -0.01f, 0, 1);
	DirectX::XMVECTOR v2 = DirectX::XMVectorSet(0.01f, -0.01f, 0, 1);
	DirectX::XMVECTOR v3 = DirectX::XMVectorSet(-0.01f, 0.01f, 0, 1);
	DirectX::XMVECTOR v4 = DirectX::XMVectorSet(0.01f, 0.01f, 0, 1);

	v1 = DirectX::XMVector4Transform(v1, transform);
	v2 = DirectX::XMVector4Transform(v2, transform);
	v3 = DirectX::XMVector4Transform(v3, transform);
	v4 = DirectX::XMVector4Transform(v4, transform);

	DirectX::XMStoreFloat4(&p1, v1);
	DirectX::XMStoreFloat4(&p2, v2);
	DirectX::XMStoreFloat4(&p3, v3);
	DirectX::XMStoreFloat4(&p4, v4);

	Bullet::ArrVer newVertices;
	newVertices.vertices[0] = { p1.x, p1.y, 0, 1, 0.7176f, 0.1176f, 1 };
	newVertices.vertices[1] = { p2.x, p2.y, 0, 1, 0.7176f, 0.1176f, 1 };
	newVertices.vertices[2] = { p3.x, p3.y, 0, 1, 0.7176f, 0.1176f, 1 };
	newVertices.vertices[3] = { p4.x, p4.y, 0, 1, 0.7176f, 0.1176f, 1 };

	
	return newVertices;
}

float * Bullet::getPos() 
{
	if (point) 
	{
		point[0] = x;
		point[1] = y;
		return point;
	}
	else
	{
		point = new float[2];
		point[0] = x;
		point[1] = y;
		return point;
	}
}
