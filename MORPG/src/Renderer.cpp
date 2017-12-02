#include "Renderer.hpp"

Renderer::Renderer(MessageSystem * l_messageSystem, GameObjectContainer * l_gameObjects, Window* l_window) : Observer(System::S_Render, l_messageSystem){
	m_gameObjects = l_gameObjects;
	m_window = l_window;
	if (!m_font.loadFromFile("arial.ttf")) {
		printf("Error loading font\n");
	}
}

Renderer::~Renderer() {}

void Renderer::Update(float l_deltaTime) {
	// Update sprite positions
	/*for (auto& it = m_gameObjects->begin(); it != m_gameObjects->end(); it++) {
		GameObject* go = it->second;

		if (!go->IsDrawable()) {
			continue;
		}

		sf::Sprite& sprite = go->GetSprite();
		sprite.setPosition(go->GetPosition() + go->m_spriteOffset);
	}*/
}

void Renderer::Render() {

	for (auto& it = m_gameObjects->begin(); it != m_gameObjects->end(); it++) {
		GameObject* go = it->second;

		if (!go->IsDrawable()) {
			continue;
		}

		m_window->Draw(go->GetSprite());
		sf::Text& text = go->GetNameText();
		text.setFont(m_font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		m_window->Draw(text);
	}
}

void Renderer::Notify(Message l_message) {}
