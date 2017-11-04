#pragma once

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Window {

public:
	Window(const std::string& l_title, const sf::Vector2u& l_size);
	~Window();

	void BeginDraw(); // Clear the window.
	void EndDraw(); // Display the changes.

	void Update();

	bool IsDone();
	bool IsFocused();

	void Close();

	void Draw(sf::Drawable& l_drawable);

	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow();

private:
	void Setup(const std::string l_title, const sf::Vector2u& l_size);
	void Create();
	void Destroy();

private:
	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isFocused;
	bool m_isDone;
};