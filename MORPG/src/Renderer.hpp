#pragma once

#include "GameObject.hpp"
#include "Observer.hpp"
#include "Definitions.hpp"
#include "Window.hpp"

class Renderer : public Observer {
public:
	Renderer(MessageSystem* l_messageSystem, GameObjectContainer* l_gameObjects, Window* l_window);
	~Renderer();

	void Update(float l_deltaTime);
	void Render();

protected:
	void Notify(Message message);

private:
	GameObjectContainer* m_gameObjects;
	Window* m_window;
};