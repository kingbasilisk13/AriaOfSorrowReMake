#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (const std::pair<std::string, Texture*>& textures : m_TextureMap)
	{
		delete textures.second;
	}
}

const Texture* TextureManager::GetTexture(const std::string& filename)
{
	Texture* result;

	std::map<std::string, Texture*>::iterator location{ m_TextureMap.find(filename) };

	if( location != m_TextureMap.end() )
	{
		result = location->second;
	}
	else
	{
		result = new Texture( filename );
		m_TextureMap[filename] = result;
	}
	return result;
}
