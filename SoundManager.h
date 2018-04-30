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
#pragma once

#include <memory>
#include "Audio.h"
#include <random>
class SoundManager {
public:
	SoundManager(float x);
	~SoundManager();
	void Update(int gamemode);
	void Suspend();
	void Resume();
	void Reset();
	float setVolume(float vol);
	float changeVolume(float delta);

	void OnNewAudioDevice() { m_retryAudio = true; }

private:
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	bool m_retryAudio;
	std::unique_ptr<DirectX::SoundEffect> m_Solitude;
	std::unique_ptr<std::mt19937> m_random;
	std::unique_ptr<DirectX::SoundEffectInstance> m_solitudeLoop;

	float masterVolume;
	float masterSlide;

};