#pragma once

/*
#ifndef _BITMAPCLASS_H_
#define _BITMAPCLASS_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include "textureclass.h"


class BitmapClass
{
public:
BitmapClass();
BitmapClass(const BitmapClass&);
~BitmapClass();

bool Initialize(ID3D11Device*, int, int, WCHAR*, int, int);
void Shutdown();
bool Render(ID3D11DeviceContext*, int, int);

int GetIndexCount();
ID3D11ShaderResourceView* GetTexture();
private:
struct VertexType
{
DirectX::XMFLOAT3 position;
DirectX::XMFLOAT2 texture;
};
bool InitializeBuffers(ID3D11Device*);
void ShutdownBuffers();
bool UpdateBuffers(ID3D11DeviceContext*, int, int);
void RenderBuffers(ID3D11DeviceContext*);

bool LoadTexture(ID3D11Device*, WCHAR*);
void ReleaseTexture();
ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
int m_vertexCount, m_indexCount;
TextureClass* m_Texture;
int m_screenWidth, m_screenHeight;
int m_bitmapWidth, m_bitmapHeight;
int m_previousPosX, m_previousPosY;
}
*/