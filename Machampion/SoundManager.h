#pragma once
#include <string>
#include <map>
#include "SDL_mixer.h"

enum sound_type {
	SOUND_MUSIC = 0,
	SOUND_SFX = 1
};

//A class that allows us to manage and display sounds
class SoundManager {
public:

	//Ensures only one instance
	static SoundManager* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new SoundManager();
		}

		return s_pInstance;
	}

	//Management functions

	bool load(std::string filename, std::string id, sound_type type);
	void playMusic(std::string id, int loop);
	void playSound(std::string id, int loop);

private:

	//Constructor/Destructor

	SoundManager();
	~SoundManager();

	//Instance
	static SoundManager* s_pInstance;

	//Sound maps

	std::map<std::string, Mix_Music*> m_music;
	std::map<std::string, Mix_Chunk*> m_sfxs;
};

typedef SoundManager TheSoundManager;