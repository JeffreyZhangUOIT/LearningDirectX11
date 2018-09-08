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
#include "TextHandler.h"
using namespace std;

TextHandler::TextHandler(Renderer& ren, int sw, int sh) {
	timer.resetTime();
	cabri32 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"cabri32.spritefont");
	vinerHand16 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"VinerHand16.spritefont");
	stencil30 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"Stencil30.spritefont");

	bellmt16 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"bellmt16.spritefont");
	bellmt18 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"bellmt18.spritefont");
	bellmt20 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"bellmt20.spritefont");
	bellmt22 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"bellmt22.spritefont");
	bellmt24 = std::make_unique<DirectX::SpriteFont>(ren.getDevice(), L"bellmt24.spritefont");
	

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(ren.getDeviceContext());
	screenWidth = sw; screenHeight = sh;

	vinerHand16->SetDefaultCharacter(L' ');
	cabri32->SetDefaultCharacter(L' ');
	stencil30->SetDefaultCharacter(L' ');
	bellmt16->SetDefaultCharacter(L' ');
	bellmt18->SetDefaultCharacter(L' ');
	bellmt20->SetDefaultCharacter(L' ');
	bellmt22->SetDefaultCharacter(L' ');
	bellmt24->SetDefaultCharacter(L' ');

	charPerSec = 50;
	srand(time(NULL));
	strcpy_s(schoolName, randomSchoolName());
	strcpy_s(mcName, "Anon ");

}

TextHandler::~TextHandler() {
	cabri32.reset();
	bellmt16.reset();
	vinerHand16.reset();
	bellmt24.reset();
	stencil30.reset();
	m_spriteBatch.reset();
}

char* TextHandler::randomSchoolName() {
	srand(time(NULL));
	int randi = rand() % 10;
	char sn[256];

	switch (randi) {
	case 0:
		memcpy(sn, "Ichigaya High \0", sizeof(sn));
		break;
	case 1:
		memcpy(sn, "Hosei High \0", sizeof(sn));
		break;
	case 2:
		memcpy(sn, "Kaisei High \0", sizeof(sn));
		break;
	case 3:
		memcpy(sn, "Miyagi High \0", sizeof(sn));
		break;
	case 4:
		memcpy(sn, "Fukui High \0", sizeof(sn));
		break;
	case 5:
		memcpy(sn, "Sunagawa High \0", sizeof(sn));
		break;
	case 6:
		memcpy(sn, "Koiwa High \0", sizeof(sn));
		break;
	case 7:
		memcpy(sn, "Ryogoku High \0", sizeof(sn));
		break;
	case 8:
		memcpy(sn, "Tachikawa High \0", sizeof(sn));
		break;
	case 9:
		memcpy(sn, "Akabane High \0", sizeof(sn));
		break;

	default:
		memcpy(sn, "Nakano High \0", sizeof(sn));
	}
	return sn;
	
}

void TextHandler::setMCName(char * name) {
	memcpy(mcName, name, sizeof(mcName));
}

void TextHandler::drawMyText(const wchar_t* output, float x, float y, DirectX::XMVECTORF32 color) {

	m_fontPos.x = ((x + 1) * screenWidth) / 2;
	m_fontPos.y = screenHeight - (((y + 1) * screenHeight) / 2);

	m_spriteBatch->Begin();

	DirectX::SimpleMath::Vector2 origin = bellmt24->MeasureString(output);
	origin.x /= 2;
	origin.y /= 2;

	bellmt24->DrawString(m_spriteBatch.get(), output,
		m_fontPos, color, 0.f, origin);
	
	m_spriteBatch->End();	
}

void TextHandler::resetMyTime() {
	timer.resetTime();
}

void TextHandler::drawDialog(char name[256], char parameters[1000], DirectX::XMVECTORF32 color) {
	char buf[13][110];
	char tempBuf[1000];
	char tempBuf2[1000];
	char tempBuf3[100] = "Some phrase that has 100 characters inside of it used for determining the ideal font size. 10 more.";
	char* token;
	char* nextTok;
	bool notFull = true;
	int currentBuf = 0;
	int pos = 0;

	memset(tempBuf, '\0', sizeof(tempBuf2));
	memset(tempBuf2, '\0', sizeof(tempBuf2));
	strcpy_s(tempBuf, parameters);
	token = strtok_s(tempBuf, " ", &nextTok);

	while (token != NULL) 
	{
		// Replace #Academy
		if (strncmp(token, "#Academy", strlen("#Academy")) == 0) 
		{
			strcat_s(tempBuf2, schoolName);
			pos = pos + strlen(schoolName);
		}

		// Replace #MC
		else if (strncmp(token, "#MC", strlen("#MC")) == 0){
			strcat_s(tempBuf2, mcName);
			pos = pos + strlen(mcName);
		}
		else {
			while (*token)
			{
				tempBuf2[pos] = *token;
				token++;
				pos++;
			}
			tempBuf2[pos] = ' ';
			pos++;
		}
		token = strtok_s(NULL, " ", &nextTok);
	}

	// Place n characters based on time since this line of dialog started.
	for (int c = 0; c < strlen(tempBuf2); c++) 
	{
		if (timer.elapsedTime() > (c/charPerSec) )
		{
			tempBuf[c] = tempBuf2[c];
		}
		else 
		{
			tempBuf[c] = '\0';
		}
	}

	token = strtok_s(tempBuf, " ", &nextTok);
	memset(buf, '\0', sizeof(char) * 13 * 110);

	// Fill buf
	while (token != NULL) 
	{
		if (strlen(token) < (100 - (strlen(buf[currentBuf]) + 2)))
		{
			strcat_s(buf[currentBuf], sizeof(buf[currentBuf]), token);
			strcat_s(buf[currentBuf], sizeof(buf[currentBuf]), " \0");
			token = strtok_s(NULL, " ", &nextTok);
		}
		else {
			currentBuf++;
		}
	}

	wchar_t * buffer2;
	int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, tempBuf3, -1, NULL, 0);
	buffer2 = new wchar_t[size];
	if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, tempBuf3, -1, buffer2, size)) {
		exit(99);
	}

	m_spriteBatch->Begin();


	// Draw dialog one line at a time, moving down every 110 characters (size of buf[currentBuf])
	while (currentBuf >= 0) {

		m_fontPos.x = (screenWidth / 24) * 3;
		m_fontPos.y = (screenHeight / 20) * (15 + currentBuf);
		wchar_t * buffer1;
		size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf[currentBuf], -1, NULL, 0);
		buffer1 = new wchar_t[size];
		if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf[currentBuf], -1, buffer1, size)) {
			exit(99);
		}

		// Literally hard-coded fake dynamic font rendering. There has to be a better way.... Like fonts with mip maps or something. Too much work for now though.
		// buffer2 is used to determine the font size, while buffer1 has the actual text to be rendererd. 
		DirectX::SimpleMath::Vector2 origin1 = bellmt24->MeasureString(buffer2);
		if (origin1.x < (screenWidth * 0.8f))
		{
			origin1.x = 0;
			origin1.y = 0;
			bellmt24->DrawString(m_spriteBatch.get(), buffer1, m_fontPos, black, 0.f, origin1);
		}
		else
		{
			origin1 = bellmt22->MeasureString(buffer2);
			if (origin1.x < (screenWidth * 0.8f))
			{
				origin1.x = 0;
				origin1.y = 0;
				bellmt22->DrawString(m_spriteBatch.get(), buffer1, m_fontPos, black, 0.f, origin1);
			}
			else
			{
				origin1 = bellmt20->MeasureString(buffer2);
				if (origin1.x < (screenWidth * 0.8f))
				{
					origin1.x = 0;
					origin1.y = 0;
					bellmt20->DrawString(m_spriteBatch.get(), buffer1, m_fontPos, black, 0.f, origin1);
				}
				else
				{
					origin1 = bellmt18->MeasureString(buffer2);
					if (origin1.x < (screenWidth * 0.8f))
					{
						origin1.x = 0;
						origin1.y = 0;
						bellmt18->DrawString(m_spriteBatch.get(), buffer1, m_fontPos, black, 0.f, origin1);
					}
					else
					{
						origin1.x = 0;
						origin1.y = 0;
						bellmt16->DrawString(m_spriteBatch.get(), buffer1, m_fontPos, black, 0.f, origin1);
					}
				}
			}
		}
		

		if (buffer1) {
			delete buffer1;
		}
		currentBuf--;
	}

	m_fontPos.x = (screenWidth / 24) * 3;
	m_fontPos.y = (screenHeight / 40) * 28;
	wchar_t * namebuf;
	size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, name, -1, NULL, 0);
	namebuf = new wchar_t[size];
	if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, name, -1, namebuf, size)) {
		exit(99);
	}

	DirectX::SimpleMath::Vector2 origin2 = stencil30->MeasureString(namebuf);

	origin2.x = 0;
	origin2.y /= 2;

	stencil30->DrawString(m_spriteBatch.get(), namebuf, m_fontPos, pansy, 0.f, origin2);

	m_spriteBatch->End();

}

void TextHandler::batchDraw(const wchar_t* output, float x, float y, float rotate, DirectX::XMVECTORF32 color) {
	m_fontPos.x = x;
	m_fontPos.y = y;

	DirectX::SimpleMath::Vector2 origin = bellmt24->MeasureString(output);
	origin.x /= 2;
	origin.y /= 2;

	bellmt24->DrawString(m_spriteBatch.get(), output,
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

void TextHandler::tempMessage(int timeToLive, float x, float y, const wchar_t* out, DirectX::XMVECTORF32 color) 
{
	textObjs elem;
	elem.x = ((x + 1) * screenWidth) / 2;
	elem.y = screenHeight - (((y + 1) * screenHeight) / 2);
	elem.color = color;
	elem.rotation = 0.0f;
	elem.string = out;
	elem.timeToLive = timeToLive;
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