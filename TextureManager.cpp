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

#include "TextureManager.h"
using namespace DirectX;

TextureManager::TextureManager(Renderer& ren)
{
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/pixelSword.dds", resource.GetAddressOf(), &swordTextureSRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/White.dds", resource.GetAddressOf(), &whiteTexSRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/burger.dds", resource.GetAddressOf(), &burgerTexSRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/Red.dds", resource.GetAddressOf(), &redTexSRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/Black.dds", resource.GetAddressOf(), &blackTexSRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/RacingMap1.dds", resource.GetAddressOf(), &racingMap1SRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/RacingMap2.dds", resource.GetAddressOf(), &racingMap2SRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/injuredSword.dds", resource.GetAddressOf(), &injuredSwordSRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/mc_car.dds", resource.GetAddressOf(), &mc_car);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/TileTextures.dds", resource.GetAddressOf(), &tiles);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/Match3Board.dds", resource.GetAddressOf(), &Match3Board);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/AssortedVehicles.dds", resource.GetAddressOf(), &assortedVehiclesSRV);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/mc_heart.dds", resource.GetAddressOf(), &mc_heart);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/projectile.dds", resource.GetAddressOf(), &projectile);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/BackgroundArray.dds", resource.GetAddressOf(), &backgrounds);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/PacmanBackground.dds", resource.GetAddressOf(), &pacmanBackground);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/TextBox.dds", resource.GetAddressOf(), &textBox);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/OptionBox.dds", resource.GetAddressOf(), &optionBox);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/PacEntities.dds", resource.GetAddressOf(), &pacEntities);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/FadeToBlack.dds", resource.GetAddressOf(), &fadeToBlack);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/CharacterSheet1.dds", resource.GetAddressOf(), &characters1);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/CharacterSheet2.dds", resource.GetAddressOf(), &characters2);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/ProgressBar.dds", resource.GetAddressOf(), &progressBar);
	CreateDDSTextureFromFile(ren.getDevice(), L"./Textures/ProgressDiamond.dds", resource.GetAddressOf(), &progressIcon);
	
	
	ren.getDeviceContext()->PSSetShaderResources(0, 1, &whiteTexSRV);
}


TextureManager::~TextureManager() {

}

void TextureManager::changeTex(Renderer& ren, int n) {
	switch (n) {
	case 0:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &mc_heart);
		break;
	case 1:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &pacmanBackground);
		break;
	case 2:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &burgerTexSRV);
		break;
	case 3:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &redTexSRV);
		break;
	case 4:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &blackTexSRV);
		break;
	case 5:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &racingMap1SRV);
		break;
	case 6:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &racingMap2SRV);
		break;
	case 7:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &mc_car);
		break;
	case 8:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &tiles);
		break;
	case 9:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &Match3Board);
		break;
	case 10:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &assortedVehiclesSRV);
		break;
	case 11:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &projectile);
		break;
		// 12 is free
	case 13:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &backgrounds);
		break;
	case 14:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &textBox);
		break;
	case 15:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &optionBox);
		break;
	case 16:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &pacEntities);
		break;
	case 17:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &fadeToBlack);
		break;
	case 18:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &characters1);
		break;
	case 19:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &characters2);
		break;
	case 20:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &progressBar);
		break;
	case 21:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &progressIcon);
		break;

	default:
		ren.getDeviceContext()->PSSetShaderResources(0, 1, &whiteTexSRV);
	}
}