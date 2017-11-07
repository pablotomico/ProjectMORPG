#include <iostream>
#include "Window.hpp"
#include "Definitions.hpp"
#include "MessageSystem.hpp"
#include "Input.hpp"

Window::Window(MessageSystem* l_messageSystem, const std::string& l_title, const sf::Vector2u& l_size, Input* l_inputSystem)
	: Observer(System::S_Window, l_messageSystem) {
	m_inputSystem = l_inputSystem;
	Setup(l_title, l_size);
}

Window::~Window() {
	Destroy();
}

void Window::BeginDraw() {
	m_window.clear(sf::Color::Black);
}

void Window::EndDraw() {
	m_window.display();
}

void Window::Update(float l_deltaTime) {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			Close();
		} else if (event.type == sf::Event::LostFocus) {
			m_isFocused = false;
		} else if (event.type == sf::Event::GainedFocus) {
			m_isFocused = true;
		}
		m_inputSystem->HandleEvent(event);
	}
}

bool Window::IsDone() {
	return m_isDone;
}

bool Window::IsFocused() {
	return m_isFocused;
}

void Window::Close() {
	m_isDone = true;
}

void Window::Draw(sf::Drawable & l_drawable) {
	m_window.draw(l_drawable);
}

sf::Vector2u Window::GetWindowSize() {
	return m_windowSize;
}

sf::RenderWindow* Window::GetRenderWindow() {
	return &m_window;
}

void Window::Notify(Message message) {
	if (message.m_type == MessageType::KeyPressed) {
		
		if (message.m_keyCode == sf::Keyboard::Escape) {
			Close();
		}
	}
}

void Window::Setup(const std::string l_title, const sf::Vector2u & l_size) {
	m_windowTitle = l_title;
	m_windowSize = l_size;
	m_isDone = false;

	m_isFocused = true;
	Create();
}

void Window::Create() {
	m_window.create({ m_windowSize.x, m_windowSize.y, 32 }, m_windowTitle, sf::Style::Default);

	if (CAP_FRAMERATE) {
		m_window.setFramerateLimit(MAX_FRAMERATE);
	}
}

void Window::Destroy() {
	m_window.close();
}
