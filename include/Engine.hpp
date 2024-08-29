#pragma once

//https://www.youtube.com/watch?v=3elKoMMuJBA

#include "DllBase.hpp"
#include "filesystem.hpp"
#include "MACROS.hpp" // Makes key names easier
#include "Controller.hpp" // Handler for user input
#include "Algorithms.hpp"
#include "Fundamentals.hpp"
#include <string>
#include <vector> // Dynamic Arrays
#include <iostream> // Debugging output
#include <chrono> // Timestamps
#include <cmath> // For vector math
#include <algorithm> // For sort

namespace engine {
	class Game;
	class GameObject;
	class Transform;
	class LoadableTexture;
	class ShapeComponent;
	struct AnimationNode;
	class Animator;

	long long getTimens();
	std::string boolToString(bool b);
	sf::Color ENGINE_API changeAlpha(sf::Color *color, int alpha);

	/* Holds data for a line */
	class ENGINE_API Line : public Renderable {
	public:
		sf::Vector2f start;
		sf::Vector2f end;
		sf::Color color;
		float thickness = 1.0f;
		float magnitude = 35.0f;
		Line();
		Line(sf::Vector2f begin, sf::Vector2f stop, sf::Color color);
	};

	/* Class for sf::Text construction */
	class ENGINE_API Text : public Renderable {
	public:
		sf::Vector2f position = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f origin = sf::Vector2f(0.0f, 0.0f);
		sf::Color color = sf::Color(255, 255, 255);
		std::string textString = "";
		int charSize = 24;
		sf::Font* font = nullptr;
		Text();
		~Text();
		Text(sf::Color col, sf::Vector2f pos, sf::Vector2f org, std::string text, int size, sf::Font* fontptr);
	};

	/* Game Management */

	/* Collision */

	class ENGINE_API Collider : public Renderable {
	public:
		Collider();
		int layer = 0;
		bool isCircle = false;
		bool isOverlapped = false;
		bool isRigid = false;
	};

	class PolygonCollider;

	// Namespace for collision management;
	// Seperate namespace bc I expect a lot of stuff to go here
	namespace collisions {
		// Functions
		bool overlap1D(float a1, float a2, float b1, float b2);

		// Classes
		class ENGINE_API CollisionPair {
		public:
			PolygonCollider* c1 = nullptr;
			PolygonCollider* c2 = nullptr;
			float overlap = 0.0f;

			bool operator == (const CollisionPair& other) const {
				return (c1 == other.c1 && c2 == other.c2);
			}

			bool operator > (const CollisionPair& other) const {
				return (c1 > other.c1);
			}

			bool operator < (const CollisionPair& other) const {
				return (c1 < other.c1);
			}
		};

		class ENGINE_API ColliderBound {
		public:
			float coord;
			PolygonCollider* col;
			ColliderBound(PolygonCollider* collider, float value);
			bool operator < (const engine::collisions::ColliderBound& other) const;
		};

		struct ENGINE_API Normal {
			sf::Vector2f direction;
			sf::Vector2f offset;
		};

		// Algorithms
		std::vector<CollisionPair> broadSortAndSweep(std::vector<PolygonCollider*> colliders);
		std::vector<CollisionPair> narrowSAT(std::vector<CollisionPair> pairs);

		class ENGINE_API CollisionManager {
		private:
			std::vector<CollisionPair> computedCollisions = {};
		public:
			CollisionManager();
			~CollisionManager();
			std::vector<CollisionPair> calculateCollisionPairs(std::vector<PolygonCollider*> colliders);
			void handleCollisions(std::vector<PolygonCollider*> colliders);

			std::vector<CollisionPair> getCollisions() { return computedCollisions; }
		};
	}

	namespace physics {
		//const double PI = 3.1415926535;
		const double TWOPI = PI * 2;
		const double RADTODEG = 180.0f / PI;
		const double DEGTORAD = PI / 180.0f;


		ENGINE_API double distance2D(double x1, double y1, double x2, double y2);
		ENGINE_API double distance2D(sf::Vector2f v1, sf::Vector2f v2);

		class ENGINE_API PhysicsManager {
		private:
			sf::Vector2f gravityDirection;
			float gravityScale;
		public:
			PhysicsManager();
		};

		ENGINE_API sf::Vector2f normalizeVector(sf::Vector2f v);
	}

	class ENGINE_API PolygonCollider : public Collider {
	private:
		GameObject* parentObject;
		bool isCircle;
		int circlePoints = 100;
		floatPolygon vertices = {};
	public:
		PolygonCollider();
		PolygonCollider(floatPolygon polygon, GameObject* obj);
		PolygonCollider(Rect r, GameObject* obj);
		PolygonCollider(float radius, GameObject* obj);
		~PolygonCollider();

		std::vector<collisions::Normal> getNormals();

		GameObject* getParent();
		void setParent(GameObject* obj);

		floatPolygon getPolygon();
		void setPolygon(floatPolygon p);

		bool shapeIsCircle();
		void setIsCircle(bool setTo);

		int getPoints();
		void setPoints(int points);

		// top, bottom are lowest and highest y values respectively
		// left, right are lowest and highest x values respectively
		Rect extrusion;
		void updateExtrusion();
		void updateGlobalExtrusion();
		sf::Vector2f getExtrusionsOnAxis(sf::Vector2f axis);
		sf::Vector2f getExtrusionsOnNormal(collisions::Normal normal);

		void fitToTexture(LoadableTexture* t);

		void* onCollision;
	};

	/*
		Class representing the shape to be rendered on an object
	*/
	class ENGINE_API ShapeComponent : public Renderable {
	public:
		GameObject* parentObject;
		floatPolygon vertices = {}; // y of index 1 represents radius of circle
		bool isVisible = true;
		bool isCircle = false;
		int sides = 3;

		sf::Vector2f origin;
		float rotationDegree;

		sf::Color fillColor;
		sf::Color outlineColor;
		float outlineThickness;

		LoadableTexture* texture;

		/* Constructor and destructor */
		ShapeComponent();
		ShapeComponent(floatPolygon polygon, GameObject* obj);
		ShapeComponent(float radius, GameObject* obj);
		~ShapeComponent();

		/* Public drawable shape construction functions */
		sf::ConvexShape constructShape();
	};

	class Texture {

	};

	/*
		No clue why I made this. I will probably use this for an image texture holder.
	*/
	class ENGINE_API LoadableTexture : public Renderable {
	private:
		StrippedTransform offsetTransform; // Applied on top of tranform
		Transform* transform = nullptr; // The parent's transform
		std::string texturePath; // Path to the texture
	public:
		// Constructors and destructors
		LoadableTexture();
		LoadableTexture(Transform* trans);
		LoadableTexture(Transform* trans, std::string path);

		/* Get functions */
		std::string getTexturePath();
		Transform* getTransform();
		StrippedTransform* getOffset();

		/* Set functions */
		void setTexturePath(std::string path);
		void setTransform(Transform* trans);
		void setOffset(StrippedTransform t);
	};

	struct Force {
		sf::Vector2f vector = sf::Vector2f(1,1); /* position displacement on an object of mass 1 */
		sf::Vector2f origin = sf::Vector2f(0,0); /* relative to another the object's position */
		bool persistent = false; /* is it not deleted after being applied */
	};

	/*
		A class containing information on positioning and scale
	*/
	class ENGINE_API Transform {
	private:
		/* Physics properties */
		float mass;
		float inverseMass;
	public:
		bool isPhysical = true;

		// Properties
		Transform* parentTransform = nullptr; // Not the transform of parentObject but the transform of parentObject's parent 
		GameObject* parentObject;
		sf::Vector2f size;
		sf::Vector2f origin;
		float rotationDegree;
		sf::Vector2f position;

		// Physics propertiesfloatPolygon
		sf::Vector2f velocity;
		float angularVelocity;
		PolygonCollider* col;
		std::vector<Force> actingForces;

		Transform compileParents();

		// Constructors and destructors
		Transform();
		Transform(GameObject* obj);
		~Transform();

		/* Mass */
		void calcInverseMass();

		/* Sick one liners*/
		/* Get attribute functions */
		float getMass();
		float getInverseMass();
		sf::Vector2f getSize();
		sf::Vector2f getPosition();
		sf::Vector2f getOrigin();
		float getRotation();
		GameObject* getParent();

		/* Set attribute functions */
		void setMass(float m);
		void setSize(sf::Vector2f newSize);
		void setPosition(sf::Vector2f newPosition);
		void setOrigin(sf::Vector2f newOrigin);
		void setRotation(float newAngleDegree);
		void setParent(GameObject* obj);

		/* Add to attribute functions */
		void addToSize(sf::Vector2f newSize);
		void addToPosition(sf::Vector2f newPosition);
		void addToOrigin(sf::Vector2f newOrigin);
		void addRotation(float newAngleDegree);
	};

	/*
		Game Object for use within the engine
	*/
	class ENGINE_API GameObject {
	private:
		Animator* animator;
		PolygonCollider* collider;
		Transform* transform;
		ShapeComponent* shape;
		bool isVisible = true;
		Game* engine;
		void* startFunction;
		void* updateFunction;
		void* physicsUpdateFunction;
		void* externalManager;
		GameObject* parent = nullptr;
	public:
		std::string objName = "default";
		unsigned int id;

		// Constructor and destructor
		GameObject(Game* game);
		GameObject(PolygonCollider* col, Game* game);
		GameObject(floatPolygon polygon, Game* game);
		GameObject(floatPolygon polygon, PolygonCollider* col, Game* game);
		GameObject(float radius, float colliderRadius, Game* game);
		~GameObject();
		void destroy();

		/* Get functions */
		bool getVisibility() { return isVisible; }
		PolygonCollider* getCollider();
		ShapeComponent* getShapeComponent();
		Transform* getTransform();
		void* getStartFunction();
		void* getUpdateFunction();
		void* getPhysicsUpdateFunction();
		Game* getEngine();
		Animator* getAnimator();
		GameObject* getParent();
		void* getExternalManager();

		/* Set functions */
		void setVisibility(bool v);
		void setCollider(PolygonCollider* p);
		void makeCircle(bool isCircle);
		void setStartFunction(void* f);
		void setUpdateFunction(void* f);
		void setPhysicsUpdateFunction(void* f);
		void setAnimator(Animator* a);
		void setParent(GameObject* p);
		void setExternalManager(void* manager);

		/* Misc functions */
		void updateCollider();
		void removeCollider();
		void removeShapeComponent();
	};

	class ENGINE_API Camera {
	public:
		Transform* transform;
		GameObject* focus;
	};

	class Particle;

	struct PhysicsSettings {
		float integrationFactor = 0.5f;
		sf::Vector2f gravityDir = sf::Vector2f(0,1);
		float gravitySpeed = 1.0f;
		float dragFactor = 0.9f;
		float angularDragCoef;
	};

	struct engine_settings {
		float fpsCap = 60;
		float upsCap = 120;
		sf::Uint32 windowMode = sf::Style::Titlebar | sf::Style::Close;
		sf::Vector2u windowSize = sf::Vector2u(0,0);
		bool enableWarnings = false;
	};

	class ENGINE_API AudioManager {
		
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
		long long totalFrame = 0;
		long long currentTime;
		long long lastTime = 0;
		long long lastUpdate = 0;
		long long lastPhysicsUpdate = 0;
		long long startTime;
		long long calls = 0;
		int frame = 0;
		int tickCounter = 0;
		long long lastSecond = 0;
		long long lastSecondUpdate = 0;
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
		collisions::CollisionManager* collisionManager = nullptr;

		// Buffers
		std::vector<PolygonCollider*> colliders = {};
		std::vector<Renderable*> renderableObjects = {};
		std::vector<Renderable*> uiRenderableObjects = {};
		std::vector<void*> loadedMemRef = {};

		// Gameobjects
		unsigned int nextId = 1;
		std::vector<GameObject*> gameObjects;

		// Private functions
		void init(engine_settings options);
	public:
		sf::RenderWindow* window;
		PhysicsSettings physicsSettings;

		// Constructors
		Game(engine_settings options);
		virtual ~Game();

		// Accessors
		bool isGameActive();
		const bool windowActive() const;
		bool isPaused();
		sf::Vector2f getWindowSize();


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
		bool update();
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
		void* drawShape(ShapeComponent* shape, bool isUI);
		void* drawCollider(PolygonCollider* col, bool registerThisCollider);
		void registerCollider(PolygonCollider* col);
		bool removeCollider(PolygonCollider* col);
		void* drawTexture(LoadableTexture* texture, bool isUI);
		bool removeTexture(LoadableTexture* texture);

		// Render removal functions
		bool removeFromRender(void* removePtr);
		bool removeFromUI(void* removePtr);

		void debugLog(std::string str, std::string colorStr);
		void* debugLine(sf::Vector2f start, sf::Vector2f end);

		// Object functions
		GameObject* makeObject(int layer, floatPolygon polygon, bool toUI);
		GameObject* makeObject(int layer, floatPolygon polygon, PolygonCollider* col, bool toUI);
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