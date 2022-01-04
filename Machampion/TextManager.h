#pragma once
#include "SDL_ttf.h"
#include <string>
#include <map>

//A class that allows us to manage and display text
class TextManager {
public:

	//Ensures only one instance
	static TextManager* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new TextManager();
		}

		return s_pInstance;
	}

	//Management functions

	bool load(std::string text, std::string id, std::string font, int size, int r, int g, int b, SDL_Renderer* pRenderer);
	void draw(std::string id, int x, int y, SDL_Renderer* pRenderer);
	void updateCoins();

	//Clear functions

	void clearFromTextureMap(std::string id);
	void clearTextureMap();

private:

	//Constructor/Destructor

	TextManager() {}
	~TextManager() {}

	//Instance
	static TextManager* s_pInstance;

	//Texture map
	std::map<std::string, SDL_Texture*> m_textureMap;
};

typedef TextManager TheTextManager;