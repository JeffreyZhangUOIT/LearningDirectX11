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
#include <wrl/client.h>
#include <DirectXMath.h>

class Renderer {
public:
	Renderer(Window& window);
	~Renderer();
	void beginFrame();
	void endFrame();
	CD3D11_VIEWPORT viewport;
	IDXGISwapChain* getSwapChain();

	Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	D3D11_TEXTURE2D_DESC m_pBackBufferdesc;
	void createShader();
	bool fullscreen = false;

private:
	
	void createDX10Device(Window& window);
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


	struct ConstantBuffer
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};
};