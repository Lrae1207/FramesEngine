#pragma once

//https://www.youtube.com/watch?v=3elKoMMuJBA

#include "Fundamentals.hpp"
#include "filesystem.hpp"
#include "Controller.hpp" // Handler for user input
#include "Algorithms.hpp"
#include "Collisions.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Audio.hpp"
#include "Noise.hpp"

#include <string>
#include <vector> // Dynamic Arrays
#include <iostream> // Debugging output
#include <chrono> // Timestamps
#include <cmath> // For vector math
#include <algorithm> // For sort

namespace engine {
	class Game;
	class GameObject;

	long long getTimens();
	std::string boolToString(bool b);
	sf::Color ENGINE_API changeAlpha(sf::Color *color, int alpha);

	/* Game Management */

	/* Collision */

	class ENGINE_API Camera {
	public:
		Transform* transform;
		GameObject* focus;
	};

	class Particle;

	struct PhysicsSettings {
		float integrationFactor = 0.5f;
		V2float gravityDir = V2float(0,1);
		float gravitySpeed = 1.0f;
		float airResistanceCoef = 0.9f;
		float angularDragCoef;
	};

	struct engine_settings {
		float fpsCap = 60;
		float upsCap = 120;
		sf::Uint32 windowMode = sf::Style::Titlebar | sf::Style::Close;
		sf::Vector2u windowSize = sf::Vector2u(1300,1050);
		bool enableWarnings = false;
	};

	/*
		Game Engine - contains all the functions and data for a functioning engine
	*/
	class ENGINE_API Game {
	private:
		engine_settings settings;

		/* Clocking and timing */
		bool isActive = false;
		long long tick = 0;
		long long lastUpdate;
		long long totalFrame = 0;
		long long startTime;
		float timeScale;

		float maxFPS;
		float invFPS;

		float physUPS; /* Updates per second */
		float invPhysUPS;

		sf::Event event;
		sf::VideoMode videoMode;

		bool warningsEnabled = false;

		sf::Color backgroundColor = sf::Color(255,255,255);

		// Keypress handling
		bool showColliders = false;
		bool f1Held = false;
		bool paused = false;
		bool escHeld = false;

		// Camera data
		Camera camera;

		// Collisions
		//collisions::CollisionManager* collisionManager = nullptr;

		// Buffers
		std::vector<collisions::PolygonCollider*> colliders = {};
		std::vector<Renderable*> renderableObjects = {};
		std::vector<Renderable*> uiRenderableObjects = {};
		std::vector<void*> loadedMemRef = {};

		// Gameobjects
		unsigned int nextId = 1;
		std::vector<GameObject*> gameObjects;

		// Private functions
		void init(engine_settings options);
	public:
		// Collisions
		collisions::CollisionManager* collisionManager = nullptr;
		collisions::Ray performRaycast(V2float start, V2float end, float dist);

		AudioManager* audio;
		sf::RenderWindow* window;
		PhysicsSettings physicsSettings;

		// Constructors
		Game(engine_settings options);
		virtual ~Game();

		// Accessors
		bool isGameActive();
		const bool windowActive() const;
		bool isPaused();
		V2float getWindowSize();


		// Time Accessors
		float getTimescale();
		long long getElapsedTime();
		//long long getDeltaTime();
		long long getElapsedFrames();

		// Get and set
		void setBackgroundColor(sf::Color color);

		// Camera functions
		void setCamFocus(GameObject* obj);

		// Game update and render functions
		bool update(int substeps);
		void updateObjects();
		void stopScene();
		void resetScene();
		bool render();
		void start();

		void invoke(void* func);
		void invokeObjFunction(void* func, GameObject* obj);

		void dynamicDeleteRenderable(Renderable* r);

		// Sorting algorithms
		void sortRenderables(std::vector<Renderable*>* renders);

		// UI draw functions
		void* drawText(Text* text, bool isUI);
		void* drawLine(Line* line, bool isUI);

		// Screen draw functions
		void* drawShape(Shape* shape, bool isUI);
		void* drawCollider(collisions::PolygonCollider* col, bool registerThisCollider);
		void registerCollider(collisions::PolygonCollider* col);
		bool removeCollider(collisions::PolygonCollider* col);
		void* drawTexture(LoadableTexture* texture, bool isUI);
		bool removeTexture(LoadableTexture* texture);

		// Render removal functions
		bool removeFromRender(void* removePtr);
		bool removeFromUI(void* removePtr);

		void debugLog(std::string str, std::string colorStr);

		// Object functions
		GameObject* makeObject(int layer, floatPolygon polygon, bool toUI);
		GameObject* makeObject(int layer, floatPolygon polygon, collisions::PolygonCollider* col, bool toUI);
		GameObject* makeObject(int layer, float radius, bool toUI);
		GameObject* makeObject(bool toUI);
		GameObject* makeObject();
		GameObject* registerObject(GameObject* obj, bool toUI);
		GameObject* getObject(unsigned int targId);
		bool deleteObject(GameObject* delObj);
		bool deleteObject(unsigned int targId);
		std::string generateUniqueObjectName(std::string name);
		GameObject* getCamFocus();

		// Filesystem functions
		bool loadScene(std::string path);
		GameObject* loadObject(std::string path);
		GameObject* loadObject(std::string path, GameObject* parent);
		sf::Font* loadFont(std::string path);
		sf::Texture* loadTexture(std::string path);

		// Controller
		Controller controller = Controller();
	};
}