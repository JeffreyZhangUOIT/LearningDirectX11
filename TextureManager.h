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

#include "Renderer.h"
#include "DDSTextureLoader.h"
#include "SimpleMath.h"
#include "Effects.h"

class TextureManager
{

public:
	TextureManager(Renderer& ren);
	~TextureManager();
	void changeTex(Renderer& ren, int n);

private:
	//std::unique_ptr<DirectX::EnvironmentMapEffect> m_effect;
	ID3D11ShaderResourceView * swordTextureSRV;
	ID3D11ShaderResourceView * assortedVehiclesSRV;
	ID3D11ShaderResourceView * whiteTexSRV;
	ID3D11ShaderResourceView * burgerTexSRV;
	ID3D11ShaderResourceView * redTexSRV;
	ID3D11ShaderResourceView * blackTexSRV;
	ID3D11ShaderResourceView * racingMap1SRV;
	ID3D11ShaderResourceView * racingMap2SRV;
	ID3D11ShaderResourceView * injuredSwordSRV;
	ID3D11ShaderResourceView * mc_car;
	ID3D11ShaderResourceView * tiles;
	ID3D11ShaderResourceView * Match3Board;
	ID3D11ShaderResourceView * projectile;
	ID3D11ShaderResourceView * mc_heart;
	ID3D11ShaderResourceView * characters1;
	ID3D11ShaderResourceView * characters2;
	ID3D11ShaderResourceView * backgrounds;
	ID3D11ShaderResourceView * pacmanBackground;
	ID3D11ShaderResourceView * textBox;
	ID3D11ShaderResourceView * optionBox;
	ID3D11ShaderResourceView * pacEntities;
	ID3D11ShaderResourceView * fadeToBlack;
	ID3D11ShaderResourceView * progressBar;
	ID3D11ShaderResourceView * progressIcon;
};
