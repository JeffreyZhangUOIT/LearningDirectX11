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
#pragma once

#include <memory>
#include "SpriteFont.h"
#include "SimpleMath.h"
#include "Renderer.h"
#include "Timer.h"
#include <time.h>
#include <locale>
#include <codecvt>
#include <vector>

class TextHandler {
public:
	TextHandler(Renderer& ren, int sw, int sh);
	~TextHandler();
	void drawMyText(const wchar_t* output, float x, float y, DirectX::XMVECTORF32 color);
	void batchDraw(const wchar_t* output, float x, float y, float rotate, DirectX::XMVECTORF32 color);
	void drawDialog(char name[256], char parameters[1000], DirectX::XMVECTORF32 color);
	void deathRattle( float x, float y, DirectX::XMVECTORF32 color);
	void tempMessage(int timeToLive, float x, float y, const wchar_t* output, DirectX::XMVECTORF32 color);
	char * randomSchoolName();
	void resetMyTime();
	void setMCName(char * name);
	void update();
	double charPerSec;
	char mcName[256] = "Anon ";
	char schoolName[256] = "Dud High ";

private:
	Timer timer;
	int sizeofabuffer;
	

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

	DirectX::XMVECTORF32 black = { 0, 0, 0, 1 };
	DirectX::XMVECTORF32 red = { 1, 0, 0, 1 };
	DirectX::XMVECTORF32 pansy = { 72 / 255.0f, 221 / 255.0f, 136 / 255.0f, 1 };
	
	std::unique_ptr<DirectX::SpriteFont> cabri32;
	
	std::unique_ptr<DirectX::SpriteFont> vinerHand16;
	std::unique_ptr<DirectX::SpriteFont> bellmt16;
	std::unique_ptr<DirectX::SpriteFont> bellmt18;
	std::unique_ptr<DirectX::SpriteFont> bellmt20;
	std::unique_ptr<DirectX::SpriteFont> bellmt22;
	std::unique_ptr<DirectX::SpriteFont> bellmt24;
	std::unique_ptr<DirectX::SpriteFont> vinerHand32;
	std::unique_ptr<DirectX::SpriteFont> stencil30;
	
};
