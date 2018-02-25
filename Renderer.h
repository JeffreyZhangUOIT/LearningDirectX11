#pragma once
#include "Window.h"
#include <d3d11.h>
#include <DirectXMath.h>


class Renderer {
public:
	Renderer(Window& window);
	~Renderer();
	void beginFrame();
	void endFrame();
	IDXGISwapChain* getSwapChain();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	D3D11_TEXTURE2D_DESC m_pBackBufferdesc;
	void createShader();

private:
	void createDevice(Window& window);
	void createRenderTarget();

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