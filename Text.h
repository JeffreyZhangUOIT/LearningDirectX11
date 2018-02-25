#pragma once
#include <dwrite.h>
#include <d2d1.h>
#include "Renderer.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib") 

class Text {
public:
	Text(Renderer& ren);
	~Text();
	void CreateDeviceIndependentResources();
	void CreateTextFormat();
	void DrawMyText(float x1, float y1, float x2, float y2, const wchar_t* myTextString, int color, Renderer& ren);
	void DiscardDeviceResources();
	void CreateDeviceResources(Renderer& ren);

private:
	HRESULT hr;
	IDWriteFactory * pDWriteFactory_;
	IDWriteTextFormat* pTextFormat_;
	const wchar_t* wszText_;
	UINT32 cTextLength_;
	ID2D1Factory* pD2DFactory_;
	ID2D1RenderTarget* pRT_;
	float dpiScaleX_ = 1, dpiScaleY_ = 1;
	ID2D1SolidColorBrush* pBlackBrush_;
	ID2D1SolidColorBrush* pWhiteBrush_;
	IDWriteTextLayout* pTextLayout_;
	template <class T> void SafeRelease(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}
};