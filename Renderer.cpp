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
#include "Renderer.h"
#include <fstream>

#include <vector>


Renderer::Renderer(Window& window) {

	createDevice(window);
	createRenderTarget();
	createShader();

}

Renderer::~Renderer() {
	// Render target
	m_pSwapchain->SetFullscreenState(FALSE, NULL);
	m_pInputLayout->Release();
	m_pPixelShader->Release();
	m_pVertexShader->Release();
	m_pRenderTargetView->Release();

	// Device stuff
	m_pConstantBuffer->Release();
	m_pDeviceContext->Release();
	m_pDevice->Release();
	m_pSwapchain->Release();
}

void Renderer::createDevice(Window& window) {

	// swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window.getHandle();
	swapChainDesc.BufferDesc.Width = window.w;                    // set the back buffer width
	swapChainDesc.BufferDesc.Height = window.h;
	swapChainDesc.SampleDesc.Count = 1; 
	swapChainDesc.Windowed = true;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Create the swap chain device
	auto result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&m_pSwapchain, &m_pDevice, nullptr, &m_pDeviceContext);

	// Check for errors
	if (result != S_OK) {
		MessageBox(nullptr, "Problem creating DX11", "Error", MB_OK);
		exit(0);
	}
	
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, window.w, window.h, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	m_pDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf());

	
	
}

void Renderer::createRenderTarget() {
	ID3D11Texture2D* backBuffer;
	m_pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTargetView);

	backBuffer->GetDesc(&m_pBackBufferdesc); // 784 561
	backBuffer->Release();

	// Make world, view, and projection matrcies.
	using namespace DirectX;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;


	HRESULT hr; 

	hr = m_pDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
		exit(hr);

	m_pWorld = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 0.0f, -15.0f, 0.0f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_pView = DirectX::XMMatrixLookAtLH(Eye, At, Up);

	m_pProj = XMMatrixIdentity();

}

void Renderer::beginFrame() {
	
	// Bind render target
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

	// Set viewport
	viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_pBackBufferdesc.Width, (float)m_pBackBufferdesc.Height);
	m_pDeviceContext->RSSetViewports(1, &viewport);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	// Make matrix 4 shada
	ConstantBuffer cb; 
	cb.mWorld = m_pWorld;
	cb.mView = m_pView;
	cb.mProjection = m_pProj;
	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	// Set the background colour!
	float clearColor[] = { 0.0f, .0f, 0.0f, 1 };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);

}

void Renderer::endFrame() {
	// Swap the buffer these parameters have something to do with refresh rate.
	m_pSwapchain->Present(1, 0);
}

ID3D11Device* Renderer::getDevice() {
	return m_pDevice;
}

ID3D11DeviceContext* Renderer::getDeviceContext() {
	return m_pDeviceContext;
}

IDXGISwapChain* Renderer::getSwapChain() {
	return m_pSwapchain;
}

void Renderer::createShader() {
	// Create Shaders and load files.
	using namespace std;
	ifstream vsFile("VertexShader.cso", ios::binary);
	ifstream psFile("PixelShader.cso", ios::binary);

	vector<char> vsData = { istreambuf_iterator<char>(vsFile), istreambuf_iterator<char>() };
	vector<char> psData = { istreambuf_iterator<char>(psFile), istreambuf_iterator<char>() };

	m_pDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_pVertexShader);
	m_pDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_pPixelShader);

	// Create input layouts
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pDevice->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &m_pInputLayout);
}

