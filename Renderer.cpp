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

The MIT License (MIT)

Copyright (c) 2018 Microsoft Corp

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Renderer.h"
#include <fstream>
#include <vector>

void Renderer::resizeScreen(Window& window)
{
	DXGI_MODE_DESC  swapChainDesc = { 0 };
	swapChainDesc.Width = window.w;
	swapChainDesc.Height = window.h;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pSwapchain->ResizeTarget(&swapChainDesc);
	m_pSwapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, window.w, window.h, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	m_pDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf());

	createRenderTarget();
}

Renderer::Renderer(Window& window) 
{
	createDevice(window);
	createRenderTarget();
	createShader();

}

Renderer::~Renderer() {
	// Resource Buffers
	/*
	delete vertexBuf;
	delete indexBuf;
	*/
	//m_VertexBuffer1->Release();
	//m_IndexBuffer1->Release();

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
	m_depthStencilView.Reset();

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
	DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
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

	// Make matrix 4 shaders
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

void Renderer::resetFrame()
{
	// Bind render target
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

	// Set viewport
	viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_pBackBufferdesc.Width, (float)m_pBackBufferdesc.Height);
	m_pDeviceContext->RSSetViewports(1, &viewport);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	// Make matrix 4 shaders
	ConstantBuffer cb;
	cb.mWorld = m_pWorld;
	cb.mView = m_pView;
	cb.mProjection = m_pProj;
	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	m_pDevice->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &m_pInputLayout);
}

void Renderer::moveCamera(DirectX::XMFLOAT4 eye, DirectX::XMFLOAT4 at, DirectX::XMFLOAT4 up) {
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(eye.z, eye.y, eye.z, eye.w);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(at.z, at.y, at.z, at.w);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(up.z, up.y, up.z, up.w);
	m_pView = DirectX::XMMatrixLookAtLH(Eye, At, Up);
}
// Take care not to add 3 to the first triangle's indicies. Basically, best used
// when you know the total amount of triangles. Then subtract 1.
/*

void Renderer::createMeshofSquares()
{
// Create our vertext buffer
D3D11_BUFFER_DESC vertexBufferDesc;
vertexBufferDesc.ByteWidth = sizeof(vertexBuf);
vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
vertexBufferDesc.MiscFlags = 0;
vertexBufferDesc.StructureByteStride = 0;
vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
D3D11_SUBRESOURCE_DATA vertexData = { 0 };
vertexData.pSysMem = vertexBuf;

m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer1);


// Create our index buffer
D3D11_BUFFER_DESC ibd;
ibd.Usage = D3D11_USAGE_DYNAMIC;
ibd.ByteWidth = sizeof(indexBuf);
ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
ibd.MiscFlags = 0;
ibd.StructureByteStride = 0;

// Specify the data to initialize the index buffer.
D3D11_SUBRESOURCE_DATA iinitData;
iinitData.pSysMem = indexBuf;

// Create the index buffer.
m_pDevice->CreateBuffer(&ibd, &iinitData, &m_IndexBuffer1);
//m_pDeviceContext->IASetIndexBuffer(m_IndexBuffer1, DXGI_FORMAT_R32_UINT, 0);
}



// Need to always append vertex buffers before index buffers.
void Renderer::appendVertexBuffers(Renderer::Vertex vert)
{
vertexBuf[vertOffset] = vert.x;
vertOffset++;
vertexBuf[vertOffset] = vert.y;
vertOffset++;
vertexBuf[vertOffset] = vert.z;
vertOffset++;
vertexBuf[vertOffset] = vert.r;
vertOffset++;
vertexBuf[vertOffset] = vert.g;
vertOffset++;
vertexBuf[vertOffset] = vert.b;
vertOffset++;
vertexBuf[vertOffset] = vert.a;
vertOffset++;
}

void Renderer::appendIndexBuffers(DWORD idx, bool endOfObj)
{

if (endOfObj) {
indexBuf[idxOffset] = idx + idxOfObj;
idxOffset += 1;
idxOfObj = (vertOffset/ 7);
}
else {
indexBuf[idxOffset] = idx + idxOfObj;
idxOffset += 1;
}
}

void Renderer::draw() {

// Setup vertex buffer.
D3D11_MAPPED_SUBRESOURCE resource;
assert(vertexBuf != NULL);
m_pDeviceContext->Map(m_VertexBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

memcpy(resource.pData, vertexBuf, sizeof(float) * vertOffset );
m_pDeviceContext->Unmap(m_VertexBuffer1, 0);

// Setup index buffer.
assert(indexBuf != NULL);
m_pDeviceContext->Map(m_IndexBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

memcpy(resource.pData, indexBuf, sizeof(DWORD) * idxOffset);
m_pDeviceContext->Unmap(m_IndexBuffer1, 0);

// Setup topology
m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

// Bind our vertex buffer
UINT stride = sizeof(Renderer::Vertex);
UINT offset = 0;

m_pDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer1, &stride, &offset);
m_pDeviceContext->IASetIndexBuffer(m_IndexBuffer1, DXGI_FORMAT_R32_UINT, 0);

// Draw Box
m_pDeviceContext->DrawIndexed(idxOffset, 0, 0);

memcpy(vbuf, vertexBuf, sizeof(vbuf));
memcpy(ibuf, indexBuf, sizeof(ibuf));

vertOffset = 0;
idxOffset = 0;
idxOfObj = 0;
}
*/
