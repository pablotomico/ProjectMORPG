#pragma once

#include "GameObject.hpp"
#include "Observer.hpp"
#include "Definitions.hpp"
#include "Window.hpp"

class Renderer : public Observer {
public:
	Renderer(const std::shared_ptr<MessageSystem>& l_messageSystem, const std::shared_ptr<GameObjectContainer>& l_gameObjects, const std::shared_ptr<Window>& l_window);
	~Renderer();

	void Update(float l_deltaTime);
	void Render();

protected:
	void Notify(const Message& l_message);

private:
	std::shared_ptr<GameObjectContainer> m_gameObjects;
	std::shared_ptr<Window> m_window;

	sf::Font m_font;
};