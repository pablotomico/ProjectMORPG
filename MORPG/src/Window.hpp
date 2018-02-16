#pragma once

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Observer.hpp"

class Input;

class Window : public Observer {

public:
	Window(const std::shared_ptr<MessageSystem>& l_messageSystem, const std::string& l_title, const sf::Vector2u& l_size, const std::shared_ptr<Input>& l_inputSystem);
	~Window();

	void BeginDraw() noexcept; // Clear the window.
	void EndDraw(); // Display the changes.

	void Update(const float& l_deltaTime);

	bool IsDone();
	bool IsFocused();

	void Close();

	void Draw(const sf::Drawable& l_drawable);
	void UpdateCamera(GameObject* l_target);

	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow();

protected:
	void Notify(const Message& l_message);

private:
	void Setup(const std::string l_title, const sf::Vector2u& l_size);
	void Create();
	void Destroy();

private:
	std::shared_ptr<Input> m_inputSystem;

	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isFocused;
	bool m_isDone;
};