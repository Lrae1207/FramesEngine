#include "pch.h"
#include "Controller.hpp"

// Constructor and destructor
Controller::Controller() {
	mouseMoveHandler = nullptr;
	keyDownMap = std::map<sf::Keyboard::Key, bool>();
	// Initialize response functions here
}

Controller::~Controller() {

}

/* Calls the response for the given keyup event and updates the state of the key */
void Controller::handleKeyUp(const sf::Keyboard::Key key) {
	// If the key hasn't been added yet
	if (keyDownMap.find(key) == keyDownMap.end()) {
		keyDownMap.insert({ key,false });
	}
	else {
		keyDownMap[key] = false;
	}
}

/* Calls the response for the given keydown event and updates the state of the key */
void Controller::handleKeyDown(const sf::Keyboard::Key key) {
	// If the key hasn't been added yet
	if (keyDownMap.find(key) == keyDownMap.end()) {
		keyDownMap.insert({ key,true });
	}
	else {
		keyDownMap[key] = true;
	}
}

bool Controller::getKeyDown(const sf::Keyboard::Key key) {
	if (keyDownMap.find(key) == keyDownMap.end()) {
		return false;
	}
	return keyDownMap[key];
}

bool Controller::getKeyUp(const sf::Keyboard::Key key) {
	if (keyDownMap.find(key) == keyDownMap.end()) {
		return true;
	}
	return !keyDownMap[key];
}

bool Controller::getKeyDown(const sf::Mouse::Button button) {
	return sf::Mouse::isButtonPressed(button);
}

bool Controller::getKeyUp(const sf::Mouse::Button button) {
	return !sf::Mouse::isButtonPressed(button);
}


/* Calls the response for a mouse update event */
void Controller::handleMouseMove(const sf::Vector2i mouseState) {
	if (mouseMoveHandler != nullptr) {
		void (*f)(sf::Vector2i) = static_cast<void(*)(sf::Vector2i)>(mouseMoveHandler);
		(*f)(mouseState);
	}
}

sf::Vector2f Controller::getMousePosition() {
	sf::Vector2i v = sf::Mouse::getPosition();
	sf::Vector2f f = sf::Vector2f(v.x,v.y);
	return f;
}
