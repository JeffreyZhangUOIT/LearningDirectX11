#include "Text.h"


void Text::CreateDeviceIndependentResources() 
{
	
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2DFactory_
	);


	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_ISOLATED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory_)
	);

	wszText_ = L"Hello World using DirectWrite!";
	cTextLength_ = (UINT32)wcslen(wszText_);
}


void Text::CreateDeviceResources(Renderer& ren)
{
	// Create the DXGI Surface Render Target.
	FLOAT dpiX;
	FLOAT dpiY;
	pD2DFactory_->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT
		);
	// Create a Direct2D render target which can draw into the surface in the swap chain

	IDXGISurface* backBuffer;
	ren.getSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void **)&backBuffer);

	if (!pRT_)
	{
		// Create a Direct2D render target. Pointer is unable to be created correctly yet.
		hr = pD2DFactory_->CreateDxgiSurfaceRenderTarget(backBuffer, &props, &pRT_);

		// Create a black brush.
		
		hr = pRT_->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&pBlackBrush_
		);

		hr = pRT_->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&pWhiteBrush_
		);
		
	}
	
}

void Text::DiscardDeviceResources() {
	SafeRelease(&pRT_);
	SafeRelease(&pBlackBrush_);
	SafeRelease(&pWhiteBrush_);

}

void Text::CreateTextFormat() {

	hr = pDWriteFactory_->CreateTextFormat(
		L"Gabriola",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,
		L"en-us",
		&pTextFormat_
	);


	// Center align (horizontally) the text.

	hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	
	hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	


}
// x1 and y1 represent the topleft of the rectangle, while x2 and y2 represent the bottom right.

void Text::DrawMyText(float x1, float y1, float x2, float y2, const wchar_t* myTextString, int color, Renderer& ren)
{
	ID2D1SolidColorBrush* myBrush;
	if (color == 0) {
		myBrush = pWhiteBrush_;
	}
	else {
		myBrush = pBlackBrush_;
	}

	D2D1_RECT_F layoutRect = D2D1::RectF(
		x1, y1, x2, y2
	);
	wszText_ = myTextString;
	cTextLength_ = (UINT32)wcslen(wszText_);

	CreateDeviceResources(ren);
	pRT_->BeginDraw();

	//pRT_->SetTransform(D2D1::IdentityMatrix());

	//pRT_->Clear(D2D1::ColorF(D2D1::ColorF::White));
	pRT_->DrawText(
		wszText_,        // The string to render.
		cTextLength_,    // The string's length.
		pTextFormat_,    // The text format.
		layoutRect,       // The region of the window where the text will be rendered.
		myBrush     // The brush used to draw the text.
	);

	pRT_->EndDraw();
	
}

Text::Text(Renderer& ren) {
	CreateDeviceIndependentResources();
	//CreateDeviceResources(ren);
	CreateTextFormat();

	//DrawMyText(RECT rc, const wchar_t* myTextString, int color);
	
}

Text::~Text() {
	DiscardDeviceResources();
}
