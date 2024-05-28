# FramesEngine - In development
A lightweight c++ game engine library based on SFML

## Currently a Windows-only DLL but I plan to make a linux version aswell

# Examples
This is an example of the most basic code that will make the engine function
```c++
#include "Frame/Engine.hpp"

int main() {
	engine::Game g(60,120); // 60 fps max, 120 ups max

	while (g.windowActive()) {
		g.update();
		g.render();
	}
 
	return 0;
}
```

This example should generate a falling magenta triangle
```c++
#include "Frame/Engine.hpp"

int main() {
	engine::Game g(60,120);
	engine::GameObject *obj = g.makeObject();
	obj->getTransform()->setPosition(sf::Vector2f(100,100));
	g.physicsSettings.gravitySpeed = 1;
	obj->getTransform()->isPhysical = true;

	while (g.windowActive()) {
		g.render();
		g.update();
	}
 
	return 0;
}
```

# Contribution
As of right now, any help is appreciated so go ahead and report any errors and make a pull request for me to review if you have any additions.
