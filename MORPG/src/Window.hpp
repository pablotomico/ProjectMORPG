#pragma once

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Observer.hpp"
#include "GameObject.hpp"


class Input;

class Window : public Observer {

public:
	Window(MessageSystem* l_messageSystem, const std::string& l_title, const sf::Vector2u& l_size, Input* l_inputSystem);
	~Window();

	void BeginDraw(); // Clear the window.
	void EndDraw(); // Display the changes.

	void Update(float l_deltaTime);

	bool IsDone();
	bool IsFocused();

	void Close();

	void Draw(sf::Drawable& l_drawable);
	void UpdateCamera(GameObject* l_target);

	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow();

protected:
	void Notify(Message l_message);

private:
	void Setup(const std::string l_title, const sf::Vector2u& l_size);
	void Create();
	void Destroy();

private:
	Input* m_inputSystem;

	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isFocused;
	bool m_isDone;
};