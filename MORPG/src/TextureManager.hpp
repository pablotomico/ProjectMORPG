#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

class TextureManager {
public:
	TextureManager();
	~TextureManager();

	sf::Texture* GetTexture(const std::string& l_name);

	bool RequireTexture(const std::string& l_name);
	bool ReleaseTexture(const std::string& l_name);

	void PurgeTextures();


private:
	sf::Texture* Load(const std::string& l_path);
	std::pair<sf::Texture*, unsigned int>* Find(const std::string& l_name);
	bool Unload(const std::string& l_name);

private:
	std::unordered_map<std::string, std::pair<sf::Texture*, unsigned int>> m_textures;
	std::unordered_map<std::string, std::string> m_paths;
};