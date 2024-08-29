#pragma once

#include "DllBase.hpp"

// Precompile later

#include <map>


/*
	Class for controlling user inputs and facilitating interactions between user and program.
*/

class ENGINE_API Controller {
private:
	// This is a map containing all the needed inputs and whether they were pressed between the current and last update
	std::map<sf::Keyboard::Key, bool> keyDownMap = std::map<sf::Keyboard::Key, bool>();

	bool mouseDown = false;

	// Private functions
public:
	// Constructor and destructors
	Controller();
	~Controller();

	// Key state find functions for other classes
	void handleKeyUp(const sf::Keyboard::Key key);
	void handleKeyDown(const sf::Keyboard::Key key);

	// Get key down
	bool getKeyDown(const sf::Keyboard::Key key);
	bool getKeyUp(const sf::Keyboard::Key key);
	bool getKeyDown(const sf::Mouse::Button button);
	bool getKeyUp(const sf::Mouse::Button button);

	void* mouseMoveHandler;
	// Calls an external mouse update handler function
	void handleMouseMove(const sf::Vector2i mousePos);
	sf::Vector2f getMousePosition();
	// Add key hold handling
};