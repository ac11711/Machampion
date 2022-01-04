#include "TextureManager.h"
#include "SDL_image.h"
#include "CompressionHandler.h"
#include <iostream>

//Initialize static instance
TextureManager* TextureManager::s_pInstance = 0;

//Load the file into a texture
bool TextureManager::load(std::string filename, std::string id, SDL_Renderer* pRenderer) {
	//Create parsed path
	std::vector<std::string> parsedPath = parsePath(filename);
	//Create archive name
	std::string archiveName = parsedPath.front() + getArchiveNameFromParsedPath(parsedPath);
	//Replace filename with compressed file
	filename = parsedPath.back();
	//Create destination path
	std::string destPath = parsedPath.front() + filename;
	//Decompress file
	decompressFile(archiveName, filename, destPath);

	//Convert file into a surface
	SDL_Surface* pTempSurface = IMG_Load(destPath.c_str());

	//Delete extracted file
	deleteFile(destPath);

	//If surface is empty
	if (pTempSurface == 0) {
		//Display error
		std::cout << "ERROR: Could not load image - " << IMG_GetError() << std::endl;

		//Free surface pointer if load fails
		SDL_FreeSurface(pTempSurface);

		return false;
	}

	//Convert surface into a texture
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

//Draw texture to renderer
void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, int alpha, SDL_RendererFlip flip) {
	//Create rectangles
	SDL_Rect srcRect;
	SDL_Rect destRect;

	//Define rectangle dimensions
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	//Add alpha effect to texture
	SDL_SetTextureAlphaMod(m_textureMap[id], alpha);

	//Copy texture to Renderer
	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

//Draw a frame to renderer - for textures with multiple frames
void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, double angle, int alpha, SDL_RendererFlip flip) {
	//Create rectangles
	SDL_Rect srcRect;
	SDL_Rect destRect;

	//Define rectangle dimensions
	srcRect.x = width * currentFrame;
	srcRect.y = height * currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	//Add alpha effect to texture
	SDL_SetTextureAlphaMod(m_textureMap[id], alpha);
	
	//Copy texture to renderer
	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, angle, 0, flip);
}

//Draw a tile to renderer
void TextureManager::drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer) {
	//Create rectangles
	SDL_Rect srcRect;
	SDL_Rect destRect;

	//Define rectangle dimensions
	srcRect.x = margin + (spacing + width) * currentFrame;
	srcRect.y = margin + (spacing + height) * currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	//Copy texture to renderer
	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}

//Clear an ID from the texture map
void TextureManager::clearFromTextureMap(std::string id) {
	//Destroy texture pointer
	SDL_DestroyTexture(m_textureMap[id]);

	m_textureMap.erase(id);
}

//Clear the texture map
void TextureManager::clearTextureMap() {
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