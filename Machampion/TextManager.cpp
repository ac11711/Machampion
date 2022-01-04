#include "TextManager.h"
#include "Game.h"
#include <iostream>

//Initialize static instance
TextManager* TextManager::s_pInstance = 0;

//Load the the text
bool TextManager::load(std::string text, std::string id, std::string font, int size, int r, int g, int b, SDL_Renderer* pRenderer) {
	//Set font
	TTF_Font* SDLFont = TTF_OpenFont(font.c_str(), size);
	//Set color
	SDL_Color color = { r, g, b };

	//Create surface
	SDL_Surface* pTempSurface = TTF_RenderText_Solid(SDLFont, text.c_str(), color);

	//Close font
	TTF_CloseFont(SDLFont);

	//If surface is empty
	if (pTempSurface == 0) {
		//Display error
		std::cout << "ERROR: Could not load text - " << TTF_GetError() << std::endl;

		return false;
	}

	//Convert surface into texture
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);

	//Free surface pointer
	SDL_FreeSurface(pTempSurface);
	
	//If texture is not empty
	if (pTexture != 0) {
		//Add texture to texture map with ID index
		m_textureMap[id] = pTexture;

		return true;
	}

	//Destroy texture pointer if load fails
	SDL_DestroyTexture(pTexture);

	return false;
}

//Draw the text
void TextManager::draw(std::string id, int x, int y, SDL_Renderer* pRenderer) {
	//Create a rectangle
	SDL_Rect destRect;

	//Define rectangle dimensions
	destRect.x = x;
	destRect.y = y;
	destRect.w = 0;
	destRect.h = 0;
	
	//Set texture dimensions
	SDL_QueryTexture(m_textureMap[id], NULL, NULL, &destRect.w, &destRect.h);

	//Copy texture to renderer
	SDL_RenderCopy(pRenderer, m_textureMap[id], NULL, &destRect);
}

//Update coin counter
void TextManager::updateCoins() {
	//Redraw coin counter
	TheTextManager::Instance()->load(std::to_string(TheGame::Instance()->getCoins()), "coins", "RetroGame.ttf", 32, 0, 0, 0, TheGame::Instance()->getRenderer());
}

//Erase a texture
void TextManager::clearFromTextureMap(std::string id) {
	//Destroy texture pointer
	SDL_DestroyTexture(m_textureMap[id]);

	//Erase texture from texture map
	m_textureMap.erase(id);
}

//Clear texture map
void TextManager::clearTextureMap() {
	//Map iterator
	std::map<std::string, SDL_Texture*>::iterator it;

	//For each texture in texture map
	for (it = m_textureMap.begin(); it != m_textureMap.end(); it++) {
		//Destroy texture pointer
		SDL_DestroyTexture(it->second);
	}

	//Clear the texture map
	m_textureMap.clear();
}