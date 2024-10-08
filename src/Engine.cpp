#include "pch.h"
#include "Engine.hpp"

/* Get the timestamp in nanoseconds */
long long engine::getTimens() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string engine::boolToString(bool b) {
	if (b) {
		return "true";
	}
	else {
		return "false";
	}
}


engine::Text::Text()
{
}

engine::Text::~Text()
{
}

engine::Text::Text(sf::Color col, sf::Vector2f pos, sf::Vector2f org, std::string text, int size, sf::Font* fontptr) { textString = text; position = pos; origin = org; type_id = 6; layer = 0; color = col; charSize = size; font = fontptr; }

engine::ShapeComponent::ShapeComponent() {
	type_id = 5;
	parentObject = nullptr;

	fillColor = sf::Color(255, 0, 255, 255);
	outlineColor = sf::Color(0, 0, 0, 255);
	outlineThickness = 0.0f;
	rotationDegree = 0.0f;

	vertices = {};

	origin = sf::Vector2f(0.0f, 0.0f);
}

/* Default constructor and destructor for ShapeComponent */
engine::ShapeComponent::ShapeComponent(engine::floatPolygon polygon, GameObject* obj) {
	type_id = 5;
	parentObject = obj;

	fillColor = sf::Color(255, 0, 255, 255);
	outlineColor = sf::Color(0, 0, 0, 255);
	outlineThickness = 0.0f;
	rotationDegree = 0.0f;

	vertices = polygon;

	origin = sf::Vector2f(0.0f, 0.0f);
}

/* Default constructor and destructor for ShapeComponent */
engine::ShapeComponent::ShapeComponent(float radius, GameObject* obj) {
	type_id = 5;
	parentObject = obj;
	isCircle = true;

	fillColor = sf::Color(255, 0, 255, 255);
	outlineColor = sf::Color(0, 0, 0, 255);
	outlineThickness = 0.0f;
	rotationDegree = 0.0f;
	origin = sf::Vector2f(0.0f, 0.0f);
}

engine::ShapeComponent::~ShapeComponent() {

}

sf::ConvexShape engine::ShapeComponent::constructShape() {
	sf::ConvexShape polygon;

	if (!isCircle) {
		polygon.setPointCount(vertices.size());
		for (int i = 0; i < vertices.size(); ++i) {
			polygon.setPoint(i, vertices[i]);
		}
	}

	/* Set color and orientational values */
	polygon.setOrigin(origin);
	polygon.setRotation(rotationDegree);
	polygon.setFillColor(fillColor);
	polygon.setOutlineColor(outlineColor);
	polygon.setOutlineThickness(outlineThickness);

	return polygon;
}

engine::Transform engine::Transform::compileParents() {
	Transform returnTransform;
	Transform* t = this;
	returnTransform.origin = this->origin;
	while (t) {
		returnTransform.position += t->position;
		returnTransform.rotationDegree += t->rotationDegree;
		returnTransform.size.x *= t->size.x;
		returnTransform.size.y *= t->size.y;
		t = t->parentTransform;
	}
	return returnTransform;
}

engine::Transform::Transform() {
	size = sf::Vector2f(1.0f, 1.0f);
	position = sf::Vector2f(0.0f, 0.0f);
	origin = sf::Vector2f(0.0f, 0.0f);
	parentObject = nullptr;
	rotationDegree = 0;
	mass = 1;
	calcInverseMass();
}

/* Default constructor for TransformComponent */
engine::Transform::Transform(GameObject* obj) {
	size = sf::Vector2f(1.0f, 1.0f);
	position = sf::Vector2f(0.0f, 0.0f);
	origin = sf::Vector2f(0.0f, 0.0f);
	parentObject = obj;
	rotationDegree = 0;
	mass = 1;
	calcInverseMass();
	if (col != nullptr) {
		col = obj->getCollider();
	}
}

engine::Transform::~Transform() {}

void engine::Transform::calcInverseMass()
{
	inverseMass = 1.0f / mass;
}

float engine::Transform::getMass()
{
	return mass;
}

float engine::Transform::getInverseMass()
{
	return inverseMass;
}

/* Sick one liners*/
/* Get attribute functions */
sf::Vector2f engine::Transform::getSize() { return size; };
sf::Vector2f engine::Transform::getPosition() { return position; };
sf::Vector2f engine::Transform::getOrigin() { return origin; };
float engine::Transform::getRotation() { return rotationDegree; }
engine::GameObject* engine::Transform::getParent() { return parentObject; }


/* Set attribute functions */
void engine::Transform::setSize(sf::Vector2f newSize) { size = newSize; };
void engine::Transform::setPosition(sf::Vector2f newPosition) {
	position = sf::Vector2f(newPosition.x, newPosition.y);
};
void engine::Transform::setOrigin(sf::Vector2f newOrigin) { origin = newOrigin; };
void engine::Transform::setRotation(float newAngleDegree) { rotationDegree = newAngleDegree; }
void engine::Transform::setParent(GameObject* obj) { parentObject = obj; }
void engine::Transform::setMass(float m) { mass = m; calcInverseMass(); }


/* Add to attribute functions */
void engine::Transform::addToSize(sf::Vector2f newSize) { size = size + newSize; };
void engine::Transform::addToPosition(sf::Vector2f newPosition) { position = position + newPosition; };
void engine::Transform::addToOrigin(sf::Vector2f newOrigin) { origin = origin + newOrigin; };
void engine::Transform::addRotation(float newAngleDegree) { rotationDegree += newAngleDegree; }

/* GameObject constructor and destructor */
engine::GameObject::GameObject(Game* game) {
	engine = game;
	id = 0;
	shape = new ShapeComponent({ sf::Vector2f(0,0),sf::Vector2f(100,0),sf::Vector2f(100,100) }, this);
	transform = new Transform(this);
	collider = new PolygonCollider({ sf::Vector2f(0,0),sf::Vector2f(100,0),sf::Vector2f(100,100) }, this);
	startFunction = nullptr;
	updateFunction = nullptr;
	physicsUpdateFunction = nullptr;
	setVisibility(true);
	//game->drawShape(shape, false);
}

/* GameObject constructor and destructor */
engine::GameObject::GameObject(engine::floatPolygon polygon, Game* game) {
	engine = game;
	id = 0;
	shape = new ShapeComponent(polygon, this);
	transform = new Transform(this);
	collider = new PolygonCollider(polygon, this);
	startFunction = nullptr;
	updateFunction = nullptr;
	physicsUpdateFunction = nullptr;
	game->drawShape(shape, false);
}

/* GameObject constructor and destructor */
engine::GameObject::GameObject(PolygonCollider* col, Game* game) {
	engine = game;
	id = 0;
	transform = new Transform(this);
	shape = new ShapeComponent({ sf::Vector2f(0,0),sf::Vector2f(100,0),sf::Vector2f(100,100) }, this);
	collider = col;
	startFunction = nullptr;
	updateFunction = nullptr;
	physicsUpdateFunction = nullptr;
	game->drawShape(shape, false);
}

/* GameObject constructor and destructor */
engine::GameObject::GameObject(engine::floatPolygon polygon, PolygonCollider* col, Game* game) {
	engine = game;
	id = 0;
	transform = new Transform(this);
	shape = new ShapeComponent(polygon, this);
	collider = col;
	startFunction = nullptr;
	updateFunction = nullptr;
	physicsUpdateFunction = nullptr;
	game->drawShape(shape,false);
}

engine::GameObject::GameObject(float radius, float colliderRadius, Game* game) {
	engine = game;
	id = 0;
	transform = new Transform(this);
	shape = new ShapeComponent(radius, this);
	collider = new PolygonCollider(colliderRadius, this);
	shape->isCircle = true;
	collider->setIsCircle(true);
	startFunction = nullptr;
	updateFunction = nullptr;
	physicsUpdateFunction = nullptr;
	game->drawShape(shape, false);
}


engine::GameObject::~GameObject() {
	delete collider;
	delete shape;
	delete transform;
}

void engine::GameObject::destroy() {
	engine->deleteObject(this);
	delete this;
}

void engine::GameObject::setVisibility(bool v) { isVisible = v; }
void engine::GameObject::setCollider(PolygonCollider* p) { if (collider != nullptr) { delete collider; } collider = p; }
void engine::GameObject::makeCircle(bool isCircle) {
	shape->isCircle = isCircle;
	collider->setIsCircle(isCircle);
}
void engine::GameObject::setStartFunction(void* f) { startFunction = f; }
void engine::GameObject::setUpdateFunction(void* f) { updateFunction = f; }
void engine::GameObject::setPhysicsUpdateFunction(void* f) { physicsUpdateFunction = f; }

void engine::GameObject::setAnimator(Animator* a) {
	animator = a;
}

void engine::GameObject::setParent(GameObject* p) {
	parent = p;
	transform->parentTransform = p->transform;
}

void engine::GameObject::setExternalManager(void* manager) {
	externalManager = manager;
}

/* Public class functions */

engine::PolygonCollider* engine::GameObject::getCollider() {
	return collider;
}

void engine::GameObject::updateCollider() {

}

void engine::GameObject::removeCollider() {
	engine->removeFromUI(collider);
	engine->removeCollider(collider);
	delete collider;
	collider = nullptr;
}

/*
	return ShapeComponent
	Returns the shapeComponent
*/
engine::ShapeComponent* engine::GameObject::getShapeComponent() {
	return shape;
}

void engine::GameObject::removeShapeComponent() {
	engine->removeFromRender(shape);
	engine->removeFromUI(shape);
	delete shape;
	shape = nullptr;
}

engine::Transform* engine::GameObject::getTransform() {return transform;}
void* engine::GameObject::getStartFunction() { return startFunction; }
void* engine::GameObject::getUpdateFunction() { return updateFunction; }
void* engine::GameObject::getPhysicsUpdateFunction() { return physicsUpdateFunction; }

engine::Game* engine::GameObject::getEngine() {
	return engine;
}

engine::Animator* engine::GameObject::getAnimator() {
	return animator;
}

engine::GameObject* engine::GameObject::getParent() {
	return parent;
}

void* engine::GameObject::getExternalManager() {
	return externalManager;
}

/*
	return void
	Creates the window
*/
void engine::Game::init(engine_settings options) {
	startTime = getTimens();
	timeScale = 1;

	window = nullptr;
	videoMode = sf::VideoMode(options.windowSize.x, options.windowSize.y);

	window = new sf::RenderWindow(videoMode, "A game", options.windowMode);
	window->setFramerateLimit(options.fpsCap);

	camera = Camera();
	camera.transform = new Transform(nullptr); // this goes crazy
	camera.transform->setPosition(sf::Vector2f(0, 0));
	camera.transform->setSize(sf::Vector2f(window->getSize().x, window->getSize().y));

	collisionManager = new collisions::CollisionManager();
}

/*
	Constructor for game class
	Anything that happens initially goes here
*/
engine::Game::Game(engine_settings options) {
	settings = options;
	warningsEnabled = options.enableWarnings;
	maxFPS = options.fpsCap;
	invFPS = 1 / options.fpsCap;
	physUPS = options.upsCap;
	invPhysUPS = 1 / options.upsCap;

	init(options);
	debugLog("Instantiated Engine(game)", LOG_GREEN);
	debugLog("Loaded Engine(game)", LOG_GREEN);
}

engine::GameObject* engine::Game::makeObject(int layer, engine::floatPolygon polygon, bool toUI) {
	GameObject* newObject = new GameObject(polygon, this);
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	drawShape(newObject->getShapeComponent(), toUI);
	newObject->getShapeComponent()->layer = layer;
	drawCollider(newObject->getCollider(), true);
	newObject->getCollider()->layer = layer;
	return newObject;
}

engine::GameObject* engine::Game::makeObject(int layer, float radius, bool toUI) {
	GameObject* newObject = new GameObject({ sf::Vector2f(0,radius) }, this);
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);

	drawShape(newObject->getShapeComponent(), toUI);
	newObject->getShapeComponent()->layer = layer;
	drawCollider(newObject->getCollider(), true);
	newObject->getCollider()->setIsCircle(true);
	newObject->getShapeComponent()->isCircle = true;
	newObject->getCollider()->layer = layer;
	return newObject;
}

engine::GameObject* engine::Game::makeObject(bool toUI) {
	GameObject* newObject = new GameObject(this);
	newObject->id = nextId++;
	gameObjects.push_back(newObject);

	drawShape(newObject->getShapeComponent(), toUI);
	if (!toUI) {
		drawCollider(newObject->getCollider(), true);
	}
	newObject->getShapeComponent()->layer = 1;
	newObject->getCollider()->layer = 1;
	return newObject;
}

engine::GameObject* engine::Game::makeObject() {
	GameObject* newObject = new GameObject(this);
	newObject->id = nextId++;
	gameObjects.push_back(newObject);
	drawShape(newObject->getShapeComponent(), false);
	drawCollider(newObject->getCollider(), true);
	newObject->getShapeComponent()->layer = 1;
	newObject->getCollider()->layer = 1;
	return newObject;
}

engine::GameObject* engine::Game::makeObject(int layer, engine::floatPolygon polygon, PolygonCollider* col, bool toUI) {
	GameObject* newObject = new GameObject(polygon, col, this);
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	drawShape(newObject->getShapeComponent(), toUI);
	newObject->getShapeComponent()->layer = layer;
	drawCollider(newObject->getCollider(), true);
	newObject->getCollider()->layer = layer;
	return newObject;
}

engine::GameObject* engine::Game::registerObject(GameObject* obj, bool toUI) {
	obj->id = nextId;
	nextId++;
	gameObjects.push_back(obj);
	drawShape(obj->getShapeComponent(), toUI);
	drawCollider(obj->getCollider(), true);
	return obj;
}

engine::GameObject* engine::Game::getObject(unsigned int targId) {
	for (GameObject* obj : gameObjects) {
		if (obj->id == targId) {
			return obj;
		}
	}
	nullptr;
}

bool engine::Game::deleteObject(GameObject* delObj) {
	int i = 0;
	for (GameObject* obj : gameObjects) {
		if (obj == obj) {
			gameObjects.erase(gameObjects.begin() + i);
			return true;
		}
		i++;
	}
	return false;
}

bool engine::Game::deleteObject(unsigned int targId) {
	return deleteObject(getObject(targId));
}

/* Check if a given name is taken and increment its terminating number if it is */
std::string engine::Game::generateUniqueObjectName(std::string name) {
	for (GameObject* g : gameObjects) {
		if (name == g->objName) {
			size_t i = name.size() - 1;
			/* Decrement i until it finds a non-number */
			while (name[i] >= 0x30 && name[i] <= 0x39 && i >= 0) {/* 0x30='0' 0x39='9' */
				i--;
			}

			if (i == name.size()) { /* The last digit was not a number */
				name += "0";
				return name;
			}

			/* Some digits were numbers */
			std::string newName = name.substr(0, i + 1);
			int num = std::stoi(name.substr(i + 1)) + 1;
			return newName + std::to_string(num);
		}
	}
	return name;
}

engine::GameObject* engine::Game::getCamFocus() {
	return camera.focus;
}

// Destructor
engine::Game::~Game() {
	delete window;
}

bool engine::Game::isGameActive() { return isActive; }

/*
	Used for the external game loop to make sure the window isn't open
*/
const bool engine::Game::windowActive() const {
	return window->isOpen();
}
bool engine::Game::isPaused() { return paused; }
sf::Vector2f engine::Game::getWindowSize() { return vmath::utof(window->getSize()); }
float engine::Game::getTimescale() { return timeScale; }
long long engine::Game::getElapsedTime() { return getTimens() - startTime; }

long long engine::Game::getElapsedFrames() {
	return totalFrame;
}

void engine::Game::setBackgroundColor(sf::Color color) { backgroundColor = color; }

void engine::Game::setCamFocus(GameObject* obj) { camera.focus = obj; }

// Update and render functions

/*
	Updates the positions and any needed changes since last update

	- Handles events (such as keypresses)
*/
bool engine::Game::update() {
	// If a sufficient amount of time hasn't elapsed yet
	long long timestamp;
	if ((timestamp = getTimens() - lastPhysicsUpdate) < invPhysUPS * 1000000000) {
		//debugLog("t: " + std::to_string(timestamp) + " " + "u: " + std::to_string(invPhysUPS * 1000000000), LOG_RED);
		return false;
	}
	//debugLog("t: " + std::to_string(timestamp) + " " + "u: " + std::to_string(invPhysUPS * 1000000000), LOG_GREEN);

	lastPhysicsUpdate = getTimens();
	if (warningsEnabled && lastPhysicsUpdate > lastSecondUpdate + 1000000000) {
		if (tickCounter < physUPS) { 
			debugLog("Warning: update rate dropped to " + std::to_string(tickCounter) + " ups", LOG_YELLOW); 
		}
		lastSecondUpdate = lastPhysicsUpdate;
		tickCounter = 0;
	}
	tickCounter++;

	/* Event polling */
	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed: // When 'X' in top right is pressed
			window->close();
			break;
		case sf::Event::KeyPressed: // When a key press event is detected hand this off to the controller
			controller.handleKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased: // When a key release event is detected hand this off to the controller
			controller.handleKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			controller.handleMouseMove(sf::Mouse::getPosition(*window));
			break;
		/*case sf::Event::MouseButtonPressed:
			controller.setMouseDown(true);
			break;
		case sf::Event::MouseButtonReleased:
			controller.setMouseDown(false);
			break;*/
		}
	}

	/* Input handling */
	if (controller.getKeyDown(KEYCODEF1) && !f1Held) {
		showColliders = !showColliders;
		f1Held = true;
		debugLog("debugMode : " + boolToString(showColliders), LOG_CYAN);
	} else if (!controller.getKeyDown(KEYCODEF1)) {
		f1Held = false;
	}

	if (controller.getKeyDown(KEYCODEESC) && !escHeld) {
		paused = !paused;
		escHeld = true;
		debugLog("paused : " + boolToString(paused), LOG_CYAN);
	} else if (!controller.getKeyDown(KEYCODEESC)) {
		escHeld = false;
	}

	// Update camera
	sf::Vector2f offset = getWindowSize() / 2.0f;
	if (camera.focus != nullptr) {
		camera.transform->setPosition(vmath::subtractVectors(camera.focus->getTransform()->position, offset));
	}

	if (paused) {
		return false;
	}

	/* Collision handling */
	collisionManager->handleCollisions(colliders);
	for (collisions::CollisionPair pair : collisionManager->getCollisions()) {
		//debugLog("collision between: " + pair.c1->getParent()->objName + " and " + pair.c2->getParent()->objName, LOG_BLUE);
	}

	updateObjects();
	tick++;
	return true;
}

/*
	Update each individual object
*/
void engine::Game::updateObjects() {
	for (GameObject* obj : gameObjects) {
		/* Properties of the object */
		Transform* t = obj->getTransform();
		if (t == nullptr) {continue;}
		ShapeComponent* c = obj->getShapeComponent();
		if (c == nullptr) { continue; }
		PolygonCollider* p = obj->getCollider();
		if (p == nullptr) { continue; }

		/* Calculate net acceleration */
		float torque = 0;
		sf::Vector2f acceleration = sf::Vector2f(0, 0);
		for (int i = t->actingForces.size()-1; i >= 0; i--) {
			Force f = t->actingForces[i];
			acceleration += f.vector;
			torque += vmath::getMagnitude(f.origin) * vmath::getMagnitude(f.vector);
			if (!f.persistent) {
				t->actingForces.erase(t->actingForces.begin() + i);
			}
		}

		/* Gravity and other physics */
		/*if (t->isPhysical) {
			acceleration += (physicsSettings.gravityDir * physicsSettings.gravitySpeed); // gravity
		}*/

		/* Semi-implicit euler integration */
		acceleration *= t->getInverseMass(); // Make sure mass is never 0
		t->velocity += acceleration * physicsSettings.integrationFactor;// * deltaTime;

		if (t->isPhysical) {
			t->velocity *= physicsSettings.dragFactor; // calculate drag last

			t->angularVelocity *= physicsSettings.angularDragCoef;
		}

		t->position += t->velocity * physicsSettings.integrationFactor;// * deltaTime;
		t->rotationDegree += t->angularVelocity;

		/* Call update function */
		if (obj->getPhysicsUpdateFunction() != nullptr) {
			invokeObjFunction(obj->getPhysicsUpdateFunction(), obj);
		}
	}
}

void engine::Game::stopScene() {
	delete collisionManager;
	for (PolygonCollider* c : colliders) { delete c; }
	for (Renderable* r : renderableObjects) { dynamicDeleteRenderable(r); }
	for (Renderable* r : uiRenderableObjects) { dynamicDeleteRenderable(r); }
	for (GameObject* obj : gameObjects) { obj->destroy(); delete obj; }
}

void engine::Game::resetScene() {
	stopScene();
	init(settings);
}

void engine::Game::dynamicDeleteRenderable(Renderable* r) {
	switch (r->type_id) {
	case 1:
		delete (reinterpret_cast<Particle*>(r));
		break;
	case 2:
		delete (reinterpret_cast<Line*>(r));
		break;
	case 3:
		break;
	case 4:
		delete (reinterpret_cast<PolygonCollider*>(r));
		break;
	case 5:
		delete (reinterpret_cast<ShapeComponent*>(r));
		break;
	case 6:
		delete (reinterpret_cast<Text*>(r));
		break;
	}
}

/*
	Renders the window in its current state

	- Clears the screen to a color
	- Renders objects
	- Writes changes to the window
*/
bool engine::Game::render() {
	// If a sufficient amount of time hasn't elapsed yet
	long long timestamp;
	if ((timestamp = getTimens() - lastUpdate) < invFPS * 1000000000) {
		//debugLog("t: " + std::to_string(timestamp) + " " + "f: " + std::to_string(invFPS * 1000000000), LOG_RED);
		return false;
	}
	//debugLog("t: " + std::to_string(timestamp) + " " + "f: " + std::to_string(invFPS * 1000000000), LOG_GREEN);
	lastUpdate = getTimens();
	if (warningsEnabled && lastUpdate > lastSecond + 1000000000) {
		if (frame < maxFPS) { debugLog("Warning: frame rate dropped to " + std::to_string(frame) + " fps", LOG_YELLOW); };
		lastSecond = lastUpdate;
		frame = 0;
	}
	frame++;

	for (GameObject* obj : gameObjects) {
		/* Call update function */
		if (obj->getUpdateFunction() != nullptr) {
			invokeObjFunction(obj->getUpdateFunction(), obj);
		}
	}


	// Clear the screen with this color as argument
	window->clear(backgroundColor);

	
	Transform* cameraTransform = camera.transform; // Transform of the camera

	std::vector<sf::ConvexShape> drawBuffer = {};

	// Sort first by layer
	sortRenderables(&renderableObjects);

	int index = 0;
	for (Renderable* renderObj : renderableObjects) { // Convert each renderable object to a convex polygon that sfml can draw or directly draw it
		if (renderObj == nullptr) {
			renderableObjects.erase(renderableObjects.begin() + index);
			continue;
		}
		switch (renderObj->type_id) { // Change type_id to its respective type
		case renderable_id::particle:
			break;
		case renderable_id::line: {
			Line* line = reinterpret_cast<Line*>(renderObj); // Convert to Line*

			// Start, end is essentially a rect
			sf::Vector2f start = line->start;
			sf::Vector2f end = line->end;
			start.x -= line->thickness;
			start.y -= line->thickness;
			end.x += line->thickness;
			end.y += line->thickness;

			// Convert the line to a rectangle for drawing
			sf::Vector2f lineDir = sf::Vector2f(vmath::getDistance(start, end), line->magnitude);
			sf::RectangleShape drawLine(lineDir);

			drawLine.setFillColor(line->color); // Color

			// Make position relative to the camera
			drawLine.setPosition(line->start - cameraTransform->getPosition());
			lineDir = vmath::subtractVectors(line->end, line->start);

			// Rotate to proper position
			drawLine.rotate(physics::RADTODEG * atan2f(lineDir.y, lineDir.x) - 90.0f);

			window->draw(drawLine); // Draw
			break;
		}
		case renderable_id::shape: {
			ShapeComponent shapeComp = *reinterpret_cast<ShapeComponent*>(renderObj);

			if (!shapeComp.isVisible) { // Skip invisible objects
				continue;
			}

			if (shapeComp.texture != nullptr) {
				LoadableTexture* texture = shapeComp.texture;
				Transform transform = texture->getTransform()->compileParents();
				StrippedTransform* offset = texture->getOffset();
				sf::Sprite sprite;

				sf::Texture sfTexture;
				sfTexture.loadFromFile(texture->getTexturePath());
				sprite.setTexture(sfTexture);
				sprite.move(transform.getPosition() + offset->position);
				sprite.setOrigin(transform.getOrigin());
				sprite.rotate(transform.getRotation());
				sprite.setOrigin(offset->origin);
				sprite.rotate(offset->rotation);
				sprite.scale(transform.getSize());
				sprite.scale(offset->scale);

				sprite.move(camera.transform->getPosition() * -1.0f);

				sf::Vector2f textureOffset = -1.0f * vmath::utof(sfTexture.getSize());

				sprite.move(textureOffset + vmath::utof(sfTexture.getSize()));

				window->draw(sprite);
				break;
			}

			GameObject* obj = shapeComp.parentObject;
			Transform transform = obj->getTransform()->compileParents();

			if (shapeComp.isCircle) {
				sf::CircleShape circle;
				circle.setPointCount(shapeComp.sides);
				circle.setRadius(shapeComp.vertices[0].y);

				circle.setOrigin(sf::Vector2f(shapeComp.vertices[0].y, shapeComp.vertices[0].y));
				circle.move(transform.getPosition());
				circle.rotate(transform.getRotation());
				circle.scale(transform.getSize());

				circle.setFillColor(shapeComp.fillColor);

				circle.move(vmath::multiplyVector(camera.transform->getPosition(), -1));

				window->draw(circle);
			}
			else {
				sf::ConvexShape polygon = shapeComp.constructShape();
				polygon.move(transform.getPosition());
				polygon.rotate(transform.getRotation());
				polygon.scale(transform.getSize());

				polygon.move(vmath::multiplyVector(camera.transform->getPosition(), -1));

				window->draw(polygon);
			}

			break;
		}
		case renderable_id::text: {
			Text* text = reinterpret_cast<Text*>(renderObj); // Convert to Text*
			if (text->cullThis) {
				continue;
			}
			sf::Text drawText;

			drawText.setCharacterSize(text->charSize);
			drawText.setString(text->textString);
			drawText.setFont(*text->font);
			drawText.setFillColor(text->color);
			drawText.move(text->position);
			drawText.setOrigin(text->origin);

			window->draw(drawText); // Draw
			break;
		}
		case renderable_id::texture: {

		}
		default:
			continue;
		}
		index++;
	}
	
	sortRenderables(&uiRenderableObjects);
	index = 0;
	for (Renderable* renderObj : uiRenderableObjects) { // Draw UI on top
		if (renderObj == nullptr) {
			uiRenderableObjects.erase(uiRenderableObjects.begin() + index);
			index--;
			continue;
		}
		switch (renderObj->type_id) { // Change type_id to its respective type
		case renderable_id::particle:
			break;
		case renderable_id::line: {
			Line* line = reinterpret_cast<Line*>(renderObj); // Convert to Line*

			// Start, end is essentially a rect
			sf::Vector2f start = line->start;
			sf::Vector2f end = line->end;

			// Convert the line to a rectangle for drawing
			sf::Vector2f lineDir = sf::Vector2f(line->magnitude, line->thickness);
			sf::RectangleShape drawLine(lineDir);

			drawLine.setOrigin(0.0f, line->thickness * 0.5f);
			drawLine.setPosition(line->start);

			drawLine.setFillColor(line->color); // Color

			lineDir = vmath::subtractVectors(line->end, line->start);

			// Rotate to proper position
			drawLine.rotate(physics::RADTODEG * atan2f(lineDir.y, lineDir.x));

			window->draw(drawLine); // Draw
			break;
		}
		case renderable_id::circle_collider:
			break;
		case renderable_id::poly_collider: {
			if (!showColliders) {
				continue;
			}

			PolygonCollider* col = reinterpret_cast<PolygonCollider*>(renderObj);
			engine::floatPolygon polygon = col->getPolygon();
			Transform transform = col->getParent()->getTransform()->compileParents();

			if (col->shapeIsCircle()) {
				sf::CircleShape circle;
				circle.setPointCount(col->getPoints());
				circle.setRadius(col->getPolygon()[0].y);

				circle.setOrigin(sf::Vector2f(col->getPolygon()[0].y, col->getPolygon()[0].y));
				circle.move(transform.getPosition());
				circle.rotate(transform.getRotation());
				//circle.scale(transform.getSize());

				circle.setFillColor(sf::Color(0, 0, 0, 0));
				circle.setOutlineColor(sf::Color(0, 255, 0));
				circle.setOutlineThickness(5);

				circle.move(vmath::multiplyVector(camera.transform->getPosition(), -1));

				window->draw(circle);
			} else {
				sf::ConvexShape shape;
				shape.setPointCount(polygon.size());

				for (int i = 0; i < polygon.size(); ++i) {
					shape.setPoint(i, polygon[i]);
				}

				shape.move(transform.getPosition());
				shape.setOrigin(transform.getOrigin());
				shape.rotate(transform.getRotation());
				//shape.scale(transform.getSize());

				shape.setFillColor(sf::Color(0, 0, 0, 0));
				shape.setOutlineColor(sf::Color(0, 255, 0));
				shape.setOutlineThickness(5);

				shape.move(vmath::multiplyVector(camera.transform->getPosition(), -1));

				window->draw(shape);
			}

			break;
		}
		case renderable_id::shape: {
			ShapeComponent shapeComp = *reinterpret_cast<ShapeComponent*>(renderObj);

			if (!shapeComp.isVisible) { // Skip invisible objects
				continue;
			}

			if (shapeComp.texture != nullptr) {
				LoadableTexture* texture = shapeComp.texture;
				Transform transform = texture->getTransform()->compileParents();
				StrippedTransform* offset = texture->getOffset();
				sf::Sprite sprite;

				sf::Texture sfTexture;
				sfTexture.loadFromFile(texture->getTexturePath());
				sprite.setTexture(sfTexture);
				sprite.move(transform.getPosition() + offset->position);
				sprite.rotate(transform.getRotation() + offset->rotation);
				sprite.setOrigin(transform.getOrigin());
				sprite.scale(transform.getSize());
				sprite.scale(offset->scale);

				//sprite.move(camera.transform->getPosition() * -1.0f);

				sf::Vector2f textureOffset = -1.0f * vmath::utof(sfTexture.getSize());

				sprite.move(textureOffset + vmath::utof(sfTexture.getSize()));

				window->draw(sprite);
				break;
			}

			GameObject* obj = shapeComp.parentObject;
			Transform transform = obj->getTransform()->compileParents();

			if (shapeComp.isCircle) {
				sf::CircleShape circle;
				circle.setPointCount(shapeComp.sides);
				circle.setRadius(shapeComp.vertices[0].y);

				circle.setOrigin(sf::Vector2f(shapeComp.vertices[0].y, shapeComp.vertices[0].y));
				circle.move(transform.getPosition());
				circle.rotate(transform.getRotation());
				circle.scale(transform.getSize());

				window->draw(circle);
			}
			else {
				sf::ConvexShape polygon = shapeComp.constructShape();
				polygon.move(transform.getPosition());
				polygon.rotate(transform.getRotation());
				polygon.scale(transform.getSize());

				window->draw(polygon);
			}
			break;
		}
		case renderable_id::text: {
			Text* text = reinterpret_cast<Text*>(renderObj); // Convert to Text*
			if (text->cullThis) {
				continue;
			}
			sf::Text drawText;
			drawText.setCharacterSize(text->charSize);
			drawText.setString(text->textString);
			drawText.setFont(*text->font);
			drawText.setFillColor(text->color);
			drawText.move(text->position);
			drawText.setOrigin(text->origin);

			window->draw(drawText); // Draw
			break;
		}
		case renderable_id::texture: {
			LoadableTexture* texture = reinterpret_cast<LoadableTexture*>(renderObj);
			Transform transform = *texture->getTransform();
			sf::Sprite sprite;

			sf::Texture sfTexture;
			sfTexture.loadFromFile(texture->getTexturePath());
			sprite.setTexture(sfTexture);
			sprite.move(transform.getPosition());
			sprite.rotate(transform.getRotation());
			sprite.setOrigin(transform.getOrigin());
			sprite.scale(transform.getSize());

			window->draw(sprite);
			break;
		}
		default:
			continue;
		}
		index++;
	}

	// Write changes to window
	window->display();
	totalFrame++;
	return true;
}

void engine::Game::start() {
	/* Call start functions */
	for (GameObject *obj : gameObjects) {
		if (obj->getStartFunction() != nullptr) {
			invokeObjFunction(obj->getStartFunction(),obj);
		}
	}
	isActive = true;
}

void engine::Game::invoke(void* func) {
	((void(*)())func)();
}
void engine::Game::invokeObjFunction(void* func, GameObject* obj) {
	((void(*)(GameObject*))func)(obj);
}

void engine::Game::sortRenderables(std::vector<Renderable*>* renders) {
	std::sort(renders->begin(), renders->end(), [](Renderable* a, Renderable* b) {
			return a->layer < b->layer;
		});
	return;
}

void* engine::Game::drawText(Text* text, bool isUI) {
	if (text == nullptr) {
		return nullptr;
	}
	Renderable* r = dynamic_cast<Renderable*>(text);
	(isUI) ? uiRenderableObjects.push_back(r) : renderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void* engine::Game::drawLine(Line* line, bool isUI) {
	if (line == nullptr) {
		return nullptr;
	}
	Renderable* r = dynamic_cast<Renderable*>(line);
	(isUI) ? uiRenderableObjects.push_back(r) : renderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void* engine::Game::drawShape(ShapeComponent* shape, bool isUI) {
	if (shape == nullptr) {
		return nullptr;
	}
	Renderable* r = dynamic_cast<Renderable*>(shape);
	(isUI) ? uiRenderableObjects.push_back(r) : renderableObjects.push_back(r);
	return static_cast<void*>(r);
}


void* engine::Game::drawCollider(PolygonCollider* col, bool registerThisCollider) {
	if (col == nullptr) {
		return nullptr;
	}
	if (registerThisCollider) {
		registerCollider(col);
	}
	Renderable* r = dynamic_cast<Renderable*>(col);
	uiRenderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void engine::Game::registerCollider(PolygonCollider* col) {
	colliders.push_back(col);
}

void* engine::Game::drawTexture(LoadableTexture* texture, bool isUI) {
	if (texture == nullptr) {
		return nullptr;
	}
	Renderable* r = dynamic_cast<Renderable*>(texture);
	(isUI) ? uiRenderableObjects.push_back(r) : renderableObjects.push_back(r);
	return static_cast<void*>(r);
}

bool engine::Game::removeTexture(LoadableTexture* texture) {
	for (int i = 0; i < renderableObjects.size(); ++i) {
		if (renderableObjects[i] == texture) {
			renderableObjects.erase(renderableObjects.begin() + i);
			return true;
		}
	}
	for (int i = 0; i < uiRenderableObjects.size(); ++i) {
		if (uiRenderableObjects[i] == texture) {
			uiRenderableObjects.erase(uiRenderableObjects.begin() + i);
			return true;
		}
	}
	return false;
}

bool engine::Game::removeCollider(PolygonCollider* col) {
	for (int i = 0; i < colliders.size(); ++i) {
		if (colliders[i] == col) {
			colliders.erase(colliders.begin() + i);
			return true;
		}
	}
	return false;
}

bool engine::Game::removeFromRender(void* removePtr) {
	int i = 0;
	for (Renderable* r : renderableObjects) {
		if (r == removePtr) {
			renderableObjects.erase(renderableObjects.begin() + i);
			return true;
		}
		i++;
	}
	return false;
}



bool engine::Game::removeFromUI(void* removePtr) {
	int i = 0;
	for (Renderable* r : uiRenderableObjects) {
		if (r == removePtr) {
			uiRenderableObjects.erase(uiRenderableObjects.begin() + i);
			return true;
		}
		i++;
	}
	return false;
}

void engine::Game::debugLog(std::string str, std::string colorStr) {
	std::cout << "tick:" << tick << "|" << colorStr << str << LOG_RESET << std::endl;
	return;
}

void* engine::Game::debugLine(sf::Vector2f start, sf::Vector2f end) {
	Line line = Line(start, end, sf::Color::Green);
	Renderable* r = dynamic_cast<Renderable*>(&line);
	uiRenderableObjects.push_back(dynamic_cast<Renderable*>(&line));
	return static_cast<void*>(r);
}

/* Load a scene from the path of a json file */
bool engine::Game::loadScene(std::string path) {
	stopScene();
	try {
		filesystem::Parser fileReader;
		fileReader.initReader(path, filesystem::Filetype::JSON);
		std::vector<std::string> sceneFiles = fileReader.getJsonArrayFromKey<std::string>({ "sceneFiles", "objectPaths" });
		for (std::string scenePath : sceneFiles) {
			loadObject(scenePath);
		}
		debugLog("Loaded scene \"" + path + "\"", LOG_GREEN);
	}
	catch (...) {
		debugLog("Failed to load scene \"" + path + "\"", LOG_RED);
		return false;
	}
	return true;
}

/* Load an object from the path of a json file */
engine::GameObject* engine::Game::loadObject(std::string path) {
	try {
		filesystem::Parser fileReader;
		fileReader.initReader(path, filesystem::Filetype::JSON);

		std::string resourceType = fileReader.getJsonFromKey<std::string>({ "Resource","type" });

		if (resourceType == "object") {
			// Properties
			bool isCircle = fileReader.getJsonFromKey<bool>({ "General","isCircular" });
			bool isColliderNull = fileReader.getJsonFromKey<bool>({ "General", "isColliderNull" });
			bool fitCollider = fileReader.getJsonFromKey<bool>({ "General", "shouldFitCollider" });
			bool isUI = fileReader.getJsonFromKey<bool>({ "General", "isUI" });

			/* Convert JSON 2d array to list of sf::Vector2f vertices */
			std::vector<json> verticesJSON = fileReader.getJsonArrayFromKey<json>({ "MeshProperties", "vertices" });
			engine::floatPolygon vertices = {};
			for (json j : verticesJSON) {
				vertices.push_back(sf::Vector2f(j[0], j[1]));
			}
			std::vector<json> colliderJSON = fileReader.getJsonArrayFromKey<json>({ "PhysicsProperties", "colliderVertices" });
			engine::floatPolygon colliderVertices = {};
			for (json j : colliderJSON) {
				colliderVertices.push_back(sf::Vector2f(j[0], j[1]));
			}

			// Set properties
			GameObject* newObj;
			if (isCircle) {
				float radius = fileReader.getJsonFromKey<float>({ "General", "radius" });
				if (fitCollider) {
					newObj = new GameObject(radius, radius, this);
				}
				else {
					newObj = new GameObject(radius, fileReader.getJsonFromKey<float>({ "PhysicsProperties", "colliderRadius" }), this);
				}
			}
			else {
				newObj = new GameObject(vertices, this);
				if (!fitCollider) {
					PolygonCollider* unfitCollider = new PolygonCollider(colliderVertices, newObj);
					newObj->setCollider(unfitCollider);
				}
			}
			ShapeComponent* shape = newObj->getShapeComponent();
			PolygonCollider* col = newObj->getCollider();
			Transform* transform = newObj->getTransform();

			/* Set object properties */
			newObj->objName = generateUniqueObjectName(fileReader.getJsonFromKey<std::string>({ "General", "name" }));
			newObj->setVisibility(fileReader.getJsonFromKey<bool>({ "General", "isVisible" }));

			/* Set mesh properties */
			shape->sides = fileReader.getJsonFromKey<int>({ "MeshProperties", "circleApproxSides" });
			shape->fillColor = fileReader.getColorFromKey({ "MeshProperties", "fillColor" });
			shape->outlineColor = fileReader.getColorFromKey({ "MeshProperties", "outlineColor" });
			shape->outlineThickness = fileReader.getJsonFromKey<float>({ "MeshProperties", "outlineThickness" });
			shape->layer = fileReader.getJsonFromKey<int>({ "MeshProperties","layer" });

			/* Set transform properties */
			transform->setSize(fileReader.getVector2fFromKey({ "PhysicsProperties","size" }));
			transform->setOrigin(fileReader.getVector2fFromKey({ "PhysicsProperties","origin" }));
			transform->setPosition(fileReader.getVector2fFromKey({ "PhysicsProperties","position" }));
			transform->setRotation(fileReader.getJsonFromKey<float>({ "PhysicsProperties", "rotation" }));
			transform->velocity = fileReader.getVector2fFromKey({ "PhysicsProperties","velocity" });
			transform->setMass(fileReader.getJsonFromKey<float>({ "PhysicsProperties","mass" }));

			if (!fileReader.isKeyNull({ "MeshProperties","texturePath" })) {
				std::string texturePath = fileReader.getJsonFromKey<std::string>({ "MeshProperties", "texturePath" });
				sf::Texture sfTexture;
				sfTexture.loadFromFile(texturePath);
				LoadableTexture* texture = new LoadableTexture(transform, texturePath);
				shape->texture = texture;
				drawTexture(texture, isUI);
			}

			/* Set collider properties */
			if (isColliderNull) {
				newObj->setCollider(nullptr);
			}
			else {
				col->layer = fileReader.getJsonFromKey<int>({ "PhysicsProperties","layer" });
			}

			std::vector<std::string> paths = fileReader.getJsonArrayFromKey<std::string>({ "General", "children" });
			for (std::string childPath : paths) {
				loadObject(childPath, newObj);
			}

			registerObject(newObj, isUI);
			debugLog("Loaded object \"" + path + "\"", LOG_GREEN);
			return newObj;
		}
		else if (resourceType == "text") {

		}
	}
	catch (...) {
		debugLog("Failed to load object \"" + path + "\"", LOG_RED);
		return nullptr;
	}
	return nullptr;
}

engine::GameObject* engine::Game::loadObject(std::string path, GameObject* parent) {
	GameObject* g = loadObject(path);
	if (g != nullptr) {
		g->setParent(parent);
	}
	return g;
}

sf::Font* engine::Game::loadFont(std::string path) {
	sf::Font f;
	if (f.loadFromFile(path)) {
		debugLog("Loaded font resource " + path, LOG_GREEN);
		return &f;
	}
	debugLog("Failed to load font resource " + path, LOG_RED);
	return nullptr;
}

/* Do not use, creates white square problem */
sf::Texture* engine::Game::loadTexture(std::string path) {
	sf::Texture* t = new sf::Texture();
	if (t->loadFromFile(path)) {
		debugLog("Loaded texture resource " + path, LOG_GREEN);
		return t;
	}
	debugLog("Failed to load texture resource " + path, LOG_RED);
	return nullptr;
}

engine::Collider::Collider() {}

engine::PolygonCollider::PolygonCollider()
{
	vertices = {};
	type_id = 4;
	isCircle = false;
	parentObject = nullptr;
	updateExtrusion();
}

engine::PolygonCollider::PolygonCollider(engine::floatPolygon polygon, GameObject* obj) {
	vertices = polygon;
	type_id = 4;
	isCircle = false;
	parentObject = obj;
	updateExtrusion();
}

engine::PolygonCollider::PolygonCollider(Rect r, GameObject* obj) {
	type_id = 4;
	isCircle = false;
	vertices.push_back(sf::Vector2f(r.left, r.top));
	vertices.push_back(sf::Vector2f(r.right, r.top));
	vertices.push_back(sf::Vector2f(r.right, r.bottom));
	vertices.push_back(sf::Vector2f(r.left, r.bottom));
	parentObject = obj;
	updateExtrusion();
}

engine::PolygonCollider::PolygonCollider(float radius, GameObject* obj) {
	type_id = 4;
	isCircle = true;
	parentObject = obj;
	vertices.push_back(sf::Vector2f(0, radius));
}

engine::PolygonCollider::~PolygonCollider() {}

std::vector<engine::collisions::Normal> engine::PolygonCollider::getNormals() {
	std::vector<collisions::Normal> normals = {};
	if (vertices.size() == 0) {
		return normals;
	}
	sf::Vector2f transformOffset = parentObject->getTransform()->getPosition();
	for (int i = 0; i < vertices.size() - 1; ++i) {
		collisions::Normal normal;
		sf::Vector2f direction = vertices[i + 1] - vertices[i];
		normal.direction = vmath::normalizeVector(sf::Vector2f(0-direction.y, direction.x));
		normal.offset = transformOffset;
		normals.push_back(normal);
	}
	collisions::Normal normal;
	sf::Vector2f direction = vertices[0] - vertices[vertices.size() - 1];
	normal.direction = vmath::normalizeVector(sf::Vector2f(0 - direction.y, direction.x));
	normal.offset = transformOffset;
	normals.push_back(normal);
	return normals;
}

engine::GameObject* engine::PolygonCollider::getParent() { return parentObject; }
void engine::PolygonCollider::setParent(GameObject* obj) { parentObject = obj; }

engine::floatPolygon engine::PolygonCollider::getPolygon() { return vertices; }
void engine::PolygonCollider::setPolygon(floatPolygon p) { vertices = p; }

bool engine::PolygonCollider::shapeIsCircle() { return isCircle; }
void engine::PolygonCollider::setIsCircle(bool setTo) { isCircle = setTo; }

int engine::PolygonCollider::getPoints() { return circlePoints; }
void engine::PolygonCollider::setPoints(int points) { circlePoints = points; }

void engine::PolygonCollider::updateExtrusion() {
	Rect newExtrusion;
	newExtrusion.top = 0;
	newExtrusion.bottom = 0;
	newExtrusion.left = 0;
	newExtrusion.right = 0;

	if (vertices.size() == 0) {
		return;
	}

	for (sf::Vector2f v : vertices) {
		if (v.x < newExtrusion.left) {
			newExtrusion.left = v.x;
		}
		if (v.x > newExtrusion.right) {
			newExtrusion.right = v.x;
		}
		if (v.x < newExtrusion.top) {
			newExtrusion.top = v.y;
		}
		if (v.x > newExtrusion.bottom) {
			newExtrusion.bottom = v.y;
		}
	}

	extrusion = newExtrusion;
}

void engine::PolygonCollider::updateGlobalExtrusion() {
	Rect newExtrusion;
	newExtrusion.top = 0;
	newExtrusion.bottom = 0;
	newExtrusion.left = 0;
	newExtrusion.right = 0;

	if (vertices.size() == 0) {
		return;
	}

	for (sf::Vector2f v : vertices) {
		if (v.x < newExtrusion.left) {
			newExtrusion.left = v.x;
		}
		if (v.x > newExtrusion.right) {
			newExtrusion.right = v.x;
		}
		if (v.x < newExtrusion.top) {
			newExtrusion.top = v.y;
		}
		if (v.x > newExtrusion.bottom) {
			newExtrusion.bottom = v.y;
		}
	}
	Transform* t = parentObject->getTransform();
	newExtrusion.top += t->getPosition().y;
	newExtrusion.bottom += t->getPosition().y;
	newExtrusion.right += t->getPosition().x;
	newExtrusion.left += t->getPosition().x;

	extrusion = newExtrusion;
}

// Assume that a2 >= a1 and b2 >= b1
bool engine::collisions::overlap1D(float a1, float a2, float b1, float b2) {
	return std::min(a2, b2) - std::max(a1, b1) == 0;
}

engine::collisions::ColliderBound::ColliderBound(PolygonCollider* collider, float value) {
	coord = value;
	col = collider;
}

bool engine::collisions::ColliderBound::operator < (const engine::collisions::ColliderBound& other) const {
	return (coord < other.coord);
}

/* Collisions */

std::vector<engine::collisions::CollisionPair> engine::collisions::broadSortAndSweep(std::vector<PolygonCollider*> colliders) {
	std::vector<CollisionPair> pairs = {};

	std::vector<ColliderBound> xorderedBounds = {};
	std::vector<ColliderBound> yorderedBounds = {};

	/* Generate lists of bounds along x and y axis */
	for (PolygonCollider* c : colliders) {
		c->updateGlobalExtrusion();
		xorderedBounds.push_back(ColliderBound(c, c->extrusion.right));
		xorderedBounds.push_back(ColliderBound(c, c->extrusion.left));
		yorderedBounds.push_back(ColliderBound(c, c->extrusion.top));
		yorderedBounds.push_back(ColliderBound(c, c->extrusion.bottom));
	}

	std::sort(xorderedBounds.begin(), xorderedBounds.end());
	std::sort(yorderedBounds.begin(), yorderedBounds.end());
	std::vector<ColliderBound> unclosedBounds = {};

	// X coordinate
	for (ColliderBound c : xorderedBounds) {
		int i = 0;
		// Find another bound with the same collider
		while (i < unclosedBounds.size() && c.col != unclosedBounds[i].col) {
			// Add a possible CollisionPair for each unclosedBound
			CollisionPair newPair;
			ColliderBound bound = unclosedBounds[i];
			if (c.col > bound.col) { // higher collider* goes in c1; optimization for duplicate removal
				newPair.c1 = c.col;
				newPair.c2 = bound.col;
			} else {
				newPair.c2 = c.col;
				newPair.c1 = bound.col;
			}
			pairs.push_back(newPair);
			i++;
		}

		if (i == unclosedBounds.size()) { // Pair not found
			unclosedBounds.push_back(c);
		} else { // Pair found
			unclosedBounds.erase(unclosedBounds.begin() + i);
		}
	}

	unclosedBounds = {};

	// Y coordinate
	for (ColliderBound c : yorderedBounds) {
		int i = 0;
		// Find another bound with the same collider
		while (i < unclosedBounds.size() && c.col != unclosedBounds[i].col) {
			// Add a possible CollisionPair for each unclosedBound
			CollisionPair newPair;
			ColliderBound bound = unclosedBounds[i];
			if (c.col > bound.col) { // higher collider* goes in c1; optimization for duplicate removal
				newPair.c1 = c.col;
				newPair.c2 = bound.col;
			}
			else {
				newPair.c2 = c.col;
				newPair.c1 = bound.col;
			}
			pairs.push_back(newPair);
			i++;
		}

		if (i == unclosedBounds.size()) { // Pair not found
			unclosedBounds.push_back(c);
		}
		else { // Pair found
			unclosedBounds.erase(unclosedBounds.begin() + i);
		}
	}

	// Remove duplicates from pairs
	// Earlier the collider* with the higher value went in c1 and the other in c2
	// This way we don't have to cross-check c1 and c2; just if c1 == c1 && c2 == c2

	// nvm std functions
	std::sort(pairs.begin(), pairs.end());
	pairs.erase(std::unique(pairs.begin(), pairs.end()), pairs.end());

	return pairs;
}

std::vector<engine::collisions::CollisionPair> engine::collisions::narrowSAT(std::vector<CollisionPair> collisions) {
	// work herefsadsdfsadffsdfsfsdfsd


	std::vector<engine::collisions::CollisionPair> confirmedCollisions;
	for (CollisionPair pair : collisions) {
		std::vector<collisions::Normal> normal1 = pair.c1->getNormals();
		std::vector<collisions::Normal> normal2 = pair.c2->getNormals();

		// add normal2 to normal1
		// normal1 now contains all normals
		normal1.insert(normal1.end(), normal2.begin(), normal2.end());

		for (int i = 0; i < normal1.size(); ++i) {
			collisions::Normal normal = normal1[i];
			sf::Vector2f d1 = pair.c1->getExtrusionsOnNormal(normal);
			sf::Vector2f d2 = pair.c2->getExtrusionsOnNormal(normal);

			// WORK ON THIS IDK IF THIS WORKS
			//float smallestOverlap = ;

			if (!((d1.x <= d2.x && d1.y >= d2.x) || // d2.x is between d1
				(d1.x <= d2.y && d1.y >= d2.y) || // d2.y is between d1
				(d2.x <= d1.x && d2.y >= d1.x) || // d1.x is between d2
				(d2.x <= d1.y && d2.y >= d1.y))) { // d1.y is between d2
				// collision proven false
				break;
			}
			if (i == normal1.size() - 1) {// This will only be true if the program has confirmed a collision between all axes
				confirmedCollisions.push_back(pair);
			}
		}
	}
	return confirmedCollisions;
}

engine::collisions::CollisionManager::CollisionManager() {

}

engine::collisions::CollisionManager::~CollisionManager()
{
}

std::vector<engine::collisions::CollisionPair> engine::collisions::CollisionManager::calculateCollisionPairs(std::vector<PolygonCollider*> colliders) {
	return (computedCollisions = narrowSAT(broadSortAndSweep(colliders)));
}

void engine::collisions::CollisionManager::handleCollisions(std::vector<PolygonCollider*> colliders) {
	calculateCollisionPairs(colliders);

	std::sort(computedCollisions.begin(), computedCollisions.end());
	computedCollisions.erase(std::unique(computedCollisions.begin(), computedCollisions.end()), computedCollisions.end());
}

sf::Vector2f engine::PolygonCollider::getExtrusionsOnAxis(sf::Vector2f axis) {
	axis = vmath::normalizeVector(axis);
	sf::Vector2f position = parentObject->getTransform()->getPosition();

	float farthest = vmath::distanceAlongProjection(vertices[0]+position,axis);
	float closest = farthest;

	for (int i = 1; i < vertices.size(); ++i) {
		float distance = vmath::distanceAlongProjection(vertices[i]+position, axis);
		if (distance > farthest) {
			farthest = distance;
		}
		if (distance < closest) {
			closest = distance;
		}
	}
	return sf::Vector2f(closest, farthest);
}

sf::Vector2f engine::PolygonCollider::getExtrusionsOnNormal(collisions::Normal normal) {
	sf::Vector2f axis = vmath::normalizeVector(normal.direction);
	sf::Vector2f normalOffset = normal.offset;
	sf::Vector2f transformOffset = parentObject->getTransform()->getPosition() - normalOffset;

	float farthest = vmath::distanceAlongProjection(vertices[0] + transformOffset, axis);
	float closest = farthest;

	for (int i = 1; i < vertices.size(); ++i) {
		sf::Vector2f relativePos = vertices[i] + transformOffset;
		float distance = vmath::distanceAlongProjection(relativePos, axis);
		if (distance > farthest) {
			farthest = distance;
		}
		if (distance < closest) {
			closest = distance;
		}
	}
	return sf::Vector2f(closest, farthest);
}

void engine::PolygonCollider::fitToTexture(LoadableTexture* t) {
	sf::Texture sfTexture;
	sfTexture.loadFromFile(t->getTexturePath());
	StrippedTransform* offset = t->getOffset();

	sf::Vector2f textureSize = vmath::utof(sfTexture.getSize());
	Rect sizeRect;

	sizeRect.top = 0;
	sizeRect.left = 0;
	sizeRect.right = textureSize.x * offset->scale.x;
	sizeRect.bottom = textureSize.y * offset->scale.y;

	sizeRect = rotateRect(sizeRect, offset->origin, offset->rotation);
	//sizeRect = addVector(sizeRect, offset->position);

	vertices = rectToPolygon(sizeRect);
}

double engine::physics::distance2D(double x1, double y1, double x2, double y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	float d = sqrtf(dx * dx + dy * dy);
	return d;
}

double engine::physics::distance2D(sf::Vector2f v1, sf::Vector2f v2) {
	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float d = sqrtf(dx * dx + dy * dy);
	return d;
}

sf::Vector2f engine::physics::normalizeVector(sf::Vector2f v) {
	return v / (float)engine::physics::distance2D(sf::Vector2f(0,0), v);
}

engine::physics::PhysicsManager::PhysicsManager() {
	gravityDirection = sf::Vector2f(0.0,1.0);
	gravityScale = 1;
}

sf::Color engine::changeAlpha(sf::Color* color, int alpha) {
	color->a = alpha;
	return *color;
}
engine::Line::Line() {
	type_id = 2;
	start = sf::Vector2f(0.0f, 0.0f);
	end = sf::Vector2f(0.0f, 0.0f);
	color = sf::Color(0, 255, 0);
}

engine::Line::Line(sf::Vector2f begin, sf::Vector2f stop, sf::Color color) {
	type_id = 2;
	start = begin;
	end = stop;
	this->color = color;
}

engine::LoadableTexture::LoadableTexture() {
	type_id = 7; 
}

engine::LoadableTexture::LoadableTexture(Transform* trans)
{
	type_id = 7; transform = trans;
}

engine::LoadableTexture::LoadableTexture(Transform* trans, std::string path)
{
	type_id = 7; texturePath = path; transform = trans;
}

std::string engine::LoadableTexture::getTexturePath()
{
	return texturePath;
}

engine::Transform* engine::LoadableTexture::getTransform()
{
	return transform;
}

engine::StrippedTransform* engine::LoadableTexture::getOffset() {
	return &offsetTransform;
}

void engine::LoadableTexture::setTexturePath(std::string path)
{
	texturePath = path;
}

void engine::LoadableTexture::setTransform(Transform* trans)
{
	transform = trans;
}

void engine::LoadableTexture::setOffset(StrippedTransform t) {
	offsetTransform = t;
}