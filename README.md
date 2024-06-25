# FramesEngine - In development
A lightweight c++ game engine library based on SFML

## Currently a Windows-only DLL but I plan to make a linux version aswell

# The Basics

## Implementing the engine files
The engine can be utilized with by including the "Engine.hpp" file as follows:
```c++
#include "Frame/Engine.hpp"
```

**Make sure that the linker includes the "engine.lib" file** 

## Initializing the engine
Then you can itilialize an engine class like this
```c++
engine::Game g(60[^1],120[^2],false[^3]);
```
[^1]: The fps cap of the engine
[^2]: The maximum updates per second that the engine will allow. The standard is fps * 2
[^3]: true/false - allow warnings. Allowing warnings may cause some bugs.

## Creating an object instance
There are currently 5 constructors for a engine::Gameobject

`c++GameObject(Game* game);`
GameObject(PolygonCollider* col, Game* game);
GameObject(floatPolygon polygon, Game* game);
GameObject(floatPolygon polygon, PolygonCollider* col, Game* game);
GameObject(float radius, float colliderRadius, Game* game);

# Examples
This is an example of the most basic code that will make the engine function
```c++
#include "Frame/Engine.hpp"

int main() {
	engine::Game g(60,120,false); // 60 fps max, 120 ups max

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
	engine::Game g(60,120,false);
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
