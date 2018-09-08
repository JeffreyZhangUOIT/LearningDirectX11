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
#pragma once
#include "Window.h"
#include <d3d11.h>
#include <memory>
#include <DXGI.h>
#include <unordered_map>
#include <wrl/client.h>
#include <DirectXMath.h>

class Renderer {
public:
	
	struct Vertex {
		float x, y, z;
		float Xtex, Ytex;
	};

	struct SquareObj {
		Vertex vertices[4];
	};

	Renderer(Window& window);
	~Renderer();
	void beginFrame();
	
	void endFrame();
	CD3D11_VIEWPORT viewport;
	IDXGISwapChain* getSwapChain();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	D3D11_TEXTURE2D_DESC m_pBackBufferdesc;
	void createShader();
	void moveCamera(DirectX::XMFLOAT4 eye, DirectX::XMFLOAT4 at, DirectX::XMFLOAT4 up);
	void resizeScreen(Window& window);
	bool fullscreen = false;
	void resetFrame();

private:

	void createDevice(Window& window);
	void createRenderTarget();

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	// Device stuff
	IDXGISwapChain * m_pSwapchain = nullptr;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	ID3D11Buffer* m_pConstantBuffer = nullptr;

	// Render target
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	DirectX::XMMATRIX m_pWorld, m_pView, m_pProj;
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;

	/*
	ID3D11Buffer* m_VertexBuffer1 = nullptr;
	ID3D11Buffer* m_IndexBuffer1 = nullptr;
	// public
	void appendVertexBuffers(Renderer::Vertex vert);
	void appendIndexBuffers(DWORD idx, bool endOfObj);
	void updateIndicies();
	void draw();

	int vertOffset, idxOffset;
	int idxOfObj;
	float * vertexBuf;
	DWORD * indexBuf;
	float vbuf[3997];
	DWORD ibuf[571];
	*/
	
	struct ConstantBuffer
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};
};
