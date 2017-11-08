#include "Renderer.hpp"

Renderer::Renderer(MessageSystem * l_messageSystem, GameObjectContainer * l_gameObjects, Window* l_window) : Observer(System::S_Render, l_messageSystem){
	m_gameObjects = l_gameObjects;
	m_window = l_window;
}

Renderer::~Renderer() {}

void Renderer::Update(float l_deltaTime) {
	// Update sprite positions
	for (auto& it = m_gameObjects->begin(); it != m_gameObjects->end(); it++) {
		GameObject* go = it->second;

		if (!go->IsDrawable()) {
			continue;
		}

		sf::Sprite& sprite = go->GetSprite();
		sprite.setPosition(go->GetPosition());
	}
}

void Renderer::Render() {
	// TODO elevations (?)

	for (auto& it = m_gameObjects->begin(); it != m_gameObjects->end(); it++) {
		GameObject* go = it->second;

		if (!go->IsDrawable()) {
			continue;
		}

		m_window->Draw(go->GetSprite());
	}
}

void Renderer::Notify(Message l_message) {}
