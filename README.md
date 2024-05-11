# SFMLEngine
A lightweight c++ game engine library based on SFML
## !Currently a Windows only DLL but I plan to make a linux version aswell!

```c++
#include "Frame/Engine.hpp"

int main() {
	engine::Game g(60,120);

	while (g.windowActive()) {
		g.update();
		g.render();
	}
 
	return 0;
}
```
