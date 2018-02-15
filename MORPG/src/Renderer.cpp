#include "Renderer.hpp"
#include "Util\Utilities.hpp"

Renderer::Renderer(const std::shared_ptr<MessageSystem>& l_messageSystem, const std::shared_ptr<GameObjectContainer>& l_gameObjects, const std::shared_ptr<Window>& l_window)
	: Observer(System::S_Render, l_messageSystem)
	, m_gameObjects(l_gameObjects)
	, m_window(l_window) {
	if (!m_font.loadFromFile("arial.ttf")) {
		DEBUG("Error loading font");
	}
}

Renderer::~Renderer() {}

void Renderer::Update(const float& l_deltaTime) {}

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

void Renderer::Notify(const Message& l_message) {}
