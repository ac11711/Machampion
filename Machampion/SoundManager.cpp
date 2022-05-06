#include "SoundManager.h"
#include "CompressionHandler.h"
#include <iostream>

//Initialize static instance
SoundManager* SoundManager::s_pInstance = 0;

//Constructor
SoundManager::SoundManager() {
	//Open audio
	Mix_OpenAudio(22050, AUDIO_S16, 2, (4096 / 2));
}

//Destructor
SoundManager::~SoundManager() {
	//Close audio
	Mix_CloseAudio();
}

//Load file into sound map
bool SoundManager::load(std::string filename, std::string id, sound_type type) {
	//If sound is music type
	if (type == SOUND_MUSIC) {
		//Create parsed path
		std::vector<std::string> parsedPath = parsePath(filename);
		//Create archive name
		std::string archiveName = parsedPath.front() + getArchiveNameFromParsedPath(parsedPath);
		//Replace filename with new extracted file
		filename = parsedPath.back();
		//Create destination path
		std::string destPath = parsedPath.front() + filename;
		//Decompress file
		decompressFile(archiveName, filename, destPath);

		//Load music file into variable
		Mix_Music* pMusic = Mix_LoadMUS(destPath.c_str());

		//Delete extracted file
		deleteFile(destPath);

		//If music is empty
		if (pMusic == 0) {
			//Display error
			std::cout << "Could not load music: ERROR - " << Mix_GetError() << std::endl;
			
			return false;
		}

		//Add music to music map
		m_music[id] = pMusic;

		return true;
	}
	//If sound is sound fx type
	else if (type == SOUND_SFX) {
		//Create parsed path
		std::vector<std::string> parsedPath = parsePath(filename);
		//Create archive name
		std::string archiveName = parsedPath.front() + getArchiveNameFromParsedPath(parsedPath);
		//Replace filename with new extracted file
		filename = parsedPath.back();
		//Create destination path
		std::string destPath = parsedPath.front() + filename;
		//Decompress file
		decompressFile(archiveName, filename, destPath);

		//Load sound file into variable
		Mix_Chunk* pChunk = Mix_LoadWAV(destPath.c_str());

		//Delete extracted file
		deleteFile(destPath);

		//If chunk is empty
		if (pChunk == 0) {
			//Display error
			std::cout << "Could not load chunk: ERROR = " << Mix_GetError() << std::endl;

			return false;
		}

		//Add chunk to sound fx map
		m_sfxs[id] = pChunk;

		return true;
	}

	return false;
}

//Play music
void SoundManager::playMusic(std::string id, int loop) {
	Mix_PlayMusic(m_music[id], loop);
}

//Play sound
void SoundManager::playSound(std::string id, int loop) {
	Mix_PlayChannel(-1, m_sfxs[id], loop);
}

//Delete music file
void SoundManager::deleteMusic(std::string filename) {
	deleteFile(filename);
}

//Delete music and sound
void SoundManager::clearSoundMap() {
	//For each music
	for (const auto& i : m_music) {
		//Free music
		Mix_FreeMusic(i.second);
	}

	//For each sound fx
	for (const auto& i : m_sfxs) {
		//Free sfx
		Mix_FreeChunk(i.second);
	}

	//Clear maps
	m_music.clear();
	m_sfxs.clear();
}