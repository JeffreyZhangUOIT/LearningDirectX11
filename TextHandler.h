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

#include <memory>
#include "SpriteFont.h"
#include "SimpleMath.h"
#include "Renderer.h"
#include <vector>

class TextHandler {
public:
	TextHandler(Renderer& ren, int sw, int sh);
	~TextHandler();
	void drawMyText(const wchar_t* output, float x, float y, DirectX::XMVECTORF32 color);
	void batchDraw(const wchar_t* output, float x, float y, float rotate, DirectX::XMVECTORF32 color);
	void deathRattle( float x, float y, DirectX::XMVECTORF32 color);
	void grabNextLine();
	void displayNextLine();
	void update();

	std::unique_ptr<DirectX::SpriteFont> cabri32;

private:
	int screenWidth, screenHeight;
	const wchar_t* randomTextGen();
	DirectX::SimpleMath::Vector2 m_fontPos;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	struct textObjs {
		const wchar_t* string;
		float x, y, rotation; 
		DirectX::XMVECTORF32 color;
		int timeToLive;
	};

	std::vector<textObjs> timedText;
};
