#include "TextureManager.hpp"
#include "Util/Utilities.hpp"

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
	PurgeTextures();
}

sf::Texture * TextureManager::GetTexture(const std::string & l_name) {
	auto res = Find(l_name);
	return res ? res->first : nullptr;
}

bool TextureManager::RequireTexture(const std::string & l_name) {
	auto res = Find(l_name);
	if (res) {
		++res->second;
		return true;
	}
	auto path = m_paths.find(l_name);
	if (path == m_paths.end()) {
		//return false;
	}

	sf::Texture* texture = Load(l_name);
	if (!texture) {
		return false;
	}
	m_textures.emplace(l_name, std::make_pair(texture, 1));
	return true;
}

bool TextureManager::ReleaseTexture(const std::string & l_name) {
	auto res = Find(l_name);
	if (!res) {
		return false;
	}
	--res->second;
	if (!res->second) {
		Unload(l_name);
	}
	return true;
}

void TextureManager::PurgeTextures() {
	while (m_textures.begin() != m_textures.end()) {
		delete m_textures.begin()->second.first;
		m_textures.erase(m_textures.begin());
	}
}

sf::Texture * TextureManager::Load(const std::string & l_path) {
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(Utils::GetWorkingDirectory() + l_path)) {
		delete texture;
		texture = nullptr;
		DEBUG("! Failed to load texture: " << l_path);
	}
	return texture;
}

std::pair<sf::Texture*, unsigned int>* TextureManager::Find(const std::string & l_name) {
	auto itr = m_textures.find(l_name);
	return itr != m_textures.end() ? &itr->second : nullptr;
}

bool TextureManager::Unload(const std::string & l_name) {
	auto itr = m_textures.find(l_name);
	if (itr == m_textures.end()) {
		return false;
	}
	delete itr->second.first;
	m_textures.erase(itr);
	return true;
}
