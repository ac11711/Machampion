#pragma once
#include "SDL.h"
#include <string>
#include <map>

//A class that allows us to manage and display images
class TextureManager {
public:

	//Ensures only one instance
	static TextureManager* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new TextureManager();
		}

		return s_pInstance;
	}

	//Management functions

	bool load(std::string filename, std::string id, SDL_Renderer* pRenderer);
	void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, int alpha = 255, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, double angle, int alpha, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer);

	//Clear functions

	void clearFromTextureMap(std::string id);
	void clearTextureMap();

	//Get texture map
	std::map<std::string, SDL_Texture*> getTextureMap() { return m_textureMap; }

private:

	//Constructor/Destructor

	TextureManager() {}
	~TextureManager() {}

	//Instance
	static TextureManager* s_pInstance;

	//Texture map
	std::map<std::string, SDL_Texture*> m_textureMap;
};

typedef TextureManager TheTextureManager;