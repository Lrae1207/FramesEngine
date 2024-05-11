#pragma once
#include "Engine.hpp" // Include the engine

// Make any classes that you might want here
class MyObject {
public:
	int number;
	engine::GameObject* gameObject;
	void myStartFunction(engine::GameObject* g);
	void myUpdateFunction(engine::GameObject* g);
	MyObject() { number = 0; }
};