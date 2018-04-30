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
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "TextHandler.h"

TextHandler::TextHandler(Renderer& ren, int sw, int sh) {
	HRESULT hr;
	cabri32 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"cabri32.spritefont");
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(ren.getDeviceContext());
	screenWidth = sw; screenHeight = sh;
	
}

TextHandler::~TextHandler() {
	cabri32.reset();
	m_spriteBatch.reset();
}

void TextHandler::grabNextLine() {

}

void TextHandler::displayNextLine() {

}

void TextHandler::drawMyText(const wchar_t* output, float x, float y, DirectX::XMVECTORF32 color) {

	HRESULT hr;
	m_fontPos.x = x;
	m_fontPos.y = y;
	m_spriteBatch->Begin();

	DirectX::SimpleMath::Vector2 origin = cabri32->MeasureString(output);
	origin.x /= 2;
	origin.y /= 2;

	cabri32->DrawString(m_spriteBatch.get(), output,
		m_fontPos, color, 0.f, origin);
	
	m_spriteBatch->End();	
}

void TextHandler::batchDraw(const wchar_t* output, float x, float y, float rotate, DirectX::XMVECTORF32 color) {

	HRESULT hr;
	m_fontPos.x = x;
	m_fontPos.y = y;

	DirectX::SimpleMath::Vector2 origin = cabri32->MeasureString(output);
	origin.x /= 2;
	origin.y /= 2;

	cabri32->DrawString(m_spriteBatch.get(), output,
		m_fontPos, color, rotate, origin);

}

void TextHandler::deathRattle(float x, float y, DirectX::XMVECTORF32 color) {
	
	float randf = rand() % 10;
	textObjs elem;
	elem.x = ((x + 1) * screenWidth ) /2; 
	elem.y = screenHeight - (((y + 1) * screenHeight) / 2);
	elem.color = color; 
	elem.rotation = (randf / 10) - 0.5;
	elem.string = randomTextGen();
	elem.timeToLive = 50;
	timedText.emplace_back(elem);

	
}

const wchar_t* TextHandler::randomTextGen() {
	int randi = rand() % 10;
	switch (randi) {
	case 1:
		return L"The answer to the world, univerise, and everything is 42.";
		
	case 2:
		return L"MY LEG!";
		
	case 3:
		return L"AHH!";
		
	case 4:
		return L"YOU DO NOT KNO DA WAE!";
		
	case 5:
		return L"how?!";
	
	case 6:
		return L"POP!";

	case 7:
		return L"You.. monster..";
	default:
		return L"*dies*";
	}
}

void TextHandler::update() {

	m_spriteBatch->Begin();
	for (std::vector<textObjs>::iterator it = timedText.begin(); it != timedText.end();)
	{
		// For each bullet, update and draw it.
		it->timeToLive -= 1;

		if (it->timeToLive <= 0) {
			it = timedText.erase(it);
		}
		else {
			batchDraw(it->string, it->x, it->y, it->rotation, it->color);
			it++;
		}

	}
	m_spriteBatch->End();
}