#pragma once
#include <SFML/Window.hpp>

// Window
#define WINDOW_TITLE "Network MORPG"
#define WINDOW_DEFAULT_WIDTH 1920
#define WINDOW_DEFAULT_HEIGHT 1080
#define CAP_FRAMERATE true
#define MAX_FRAMERATE 60

// Camera
#define CAMERA_FOLLOW false

// Control
#define MOVEMENT_SPEED 1

// Network
#define NETWORK_TIMESTEP 50
#define SERVERIP "127.0.0.1"
#define SERVERPORT 5555

using KeyCode = sf::Keyboard::Key;