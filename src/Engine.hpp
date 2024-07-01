#pragma once

//https://www.youtube.com/watch?v=3elKoMMuJBA

#include "DllBase.hpp"
#include "filesystem.hpp"
#include "MACROS.hpp" // Makes key names easier
#include "Controller.hpp" // Handler for user input
#include <string>
#include <vector> // Dynamic Arrays
#include <iostream> // Debugging output
#include <chrono> // Timestamps
#include <cmath> // For vector math
#include <algorithm> // For sort

template <typename T>
class ENGINE_API sptr { // Smart Pointer
private:
	T* pointer; // pointer to person class
public:
	sptr() { pointer = nullptr; }

	sptr(T* guardThis) { pointer = guardThis; }

	~sptr() { if (pointer) { delete pointer; } }

	T& operator* () {
		return *pointer;
	}

	T* operator-> () {
		return pointer;
	}
};

namespace engine {
	class Game;
	class GameObject;
	class Transform;
	class Texture;
	class ShapeComponent;
	struct AnimationNode;
	class Animator;

	struct StrippedTransform {
		sf::Vector2f position = sf::Vector2f(0,0);
		float rotation = 0;
		sf::Vector2f scale = sf::Vector2f(1,1);
	};

	// Aliases
	using floatPolygon = std::vector<sf::Vector2f>;

	enum ENGINE_API renderable_id { none, particle, line, circle_collider, poly_collider, shape, text, texture };

	/* Objects that can be drawn by the engine */
	class Renderable {
	public:
		int layer = 1; // 0 is UI
		int type_id = 0; // type of object
		bool cullThis = false;
		/*
			id:
			1 - Particle
			2 - Line
			3 - RadiusCollider
			4 - PolygonCollider
			5 - ShapeComponent
			6 - Text
			7 - Sprite/texture
		*/

		bool operator < (const Renderable& other) const {
			return (layer < other.layer);
		}

		bool operator > (const Renderable& other) const {
			return (layer > other.layer);
		}
	};

	long long getTimens();
	std::string boolToString(bool b);
	sf::Color changeAlpha(sf::Color color, int alpha);

	/* Structs for data storage */

	/* Holds 4 values*/
	struct Rect {
		float top, left, bottom, right;
	};

	floatPolygon rectToPolygon(Rect r);

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

	// Vector mathmatical functions
	namespace vmath {
		sf::Vector2f ENGINE_API normalizeVector(sf::Vector2f vector);
		float ENGINE_API getMagnitude(sf::Vector2f vector);

		float ENGINE_API getDistance(sf::Vector2f v1, sf::Vector2f v2);

		sf::Vector2f ENGINE_API rotateByDegrees(sf::Vector2f vector, float degrees);

		sf::Vector2f ENGINE_API addVectors(sf::Vector2f v1, sf::Vector2f v2);
		sf::Vector2f ENGINE_API subtractVectors(sf::Vector2f v1, sf::Vector2f v2);

		float ENGINE_API dotProduct(sf::Vector2f a, sf::Vector2f b);
		float ENGINE_API distanceAlongProjection(sf::Vector2f a, sf::Vector2f b);

		sf::Vector2f ENGINE_API multiplyVector(sf::Vector2f v1, float i);
		sf::Vector2f ENGINE_API divideVector(sf::Vector2f v1, float i);

		sf::Vector2f ENGINE_API utof(sf::Vector2u v1);
	}

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
		const double PI = 3.1415926535;
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

		void fitToTexture(Texture* t);

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

		Texture* texture;

		/* Constructor and destructor */
		ShapeComponent();
		ShapeComponent(floatPolygon polygon, GameObject* obj);
		ShapeComponent(float radius, GameObject* obj);
		~ShapeComponent();

		/* Public drawable shape construction functions */
		sf::ConvexShape constructShape();
	};

	/*
		No clue why I made this. I will probably use this for an image texture holder.
	*/
	class ENGINE_API Texture : public Renderable {
	private:
		StrippedTransform offsetTransform; // Applied on top of tranform
		Transform* transform; // The parent's transform
		std::string texturePath; // Path to the texture
	public:
		// Constructors and destructors
		Texture(Transform* trans);
		Texture(Transform* trans, std::string path);

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
	public:
		GameObject* parent = nullptr;
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

		/* Set functions */
		void setVisibility(bool v);
		void setCollider(PolygonCollider* p);
		void makeCircle(bool isCircle);
		void setStartFunction(void* f);
		void setUpdateFunction(void* f);
		void setPhysicsUpdateFunction(void* f);
		void setAnimator(Animator* a);

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

	/*
		Game Engine - contains all the functions and data for a functioning engine
	*/
	class ENGINE_API Game {
	private:
		/* Clocking and timing */
		bool isActive = false;
		long long tick = 0;
		long long totalFrame = 0;
		long long currentTime;
		long long lastTime = 0;
		long long lastUpdate = 0;
		long long lastPhysicsUpdate = 0;
		long long startTime;
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

		float backgroundBrightness = 1.0f;

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

		// Gameobjects
		unsigned int nextId = 1;
		std::vector<GameObject*> gameObjects;

		// Private functions
		void init(float frameCap);
	public:
		sf::RenderWindow* window;
		PhysicsSettings physicsSettings;

		// Constructors
		Game(float fps, float ups, bool enableWarnings);
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
		void setBackgroundBrightness(float brightness);

		// Camera functions
		void setCamFocus(GameObject* obj);

		// Game update and render functions
		void update();
		void updateObjects();
		void stopScene();
		void resetScene();
		void render();
		void start();

		void invoke(void* func);
		void invokeObjFunction(void* func, GameObject* obj);

		void dynamicDeleteRenderable(Renderable* r);

		// Sorting algorithms
		std::vector<Renderable*> sortRenderables(std::vector<Renderable*> renders);

		// UI draw functions
		void* drawText(Text* text, bool isUI);
		void* drawLine(Line* line, bool isUI);

		// Screen draw functions
		void* drawShape(ShapeComponent* shape, bool isUI);
		void* drawCollider(PolygonCollider* col, bool registerThisCollider);
		void registerCollider(PolygonCollider* col);
		bool removeCollider(PolygonCollider* col);
		void* drawTexture(Texture* texture, bool isUI);
		bool removeTexture(Texture* texture);

		// Render removal functions
		bool removeFromRender(void* removePtr);
		bool removeFromUI(void* removePtr);

		void debugLog(std::string str, std::string colorStr);
		void* debugLine(sf::Vector2f start, sf::Vector2f end);

		// Object functions
		GameObject* makeObject(int layer, floatPolygon polygon, bool toUI);
		GameObject* makeObject(int layer, floatPolygon polygon, PolygonCollider* col, bool toUI);
		GameObject* makeObject(int layer, float radius, bool toUI);
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
		Controller controller;
	};

	struct ENGINE_API AnimationNode {
		float delayFrames; // delay between this node and the next node in render frames
		Texture* texture;
		StrippedTransform animationOffset;
	};

	class ENGINE_API Animator {
	private:
		Game* engine;
		ShapeComponent* shape;
		float lastFrameStamp;
		int animationPosition = 0;
		std::vector<AnimationNode*> animation = {};
	public:
		Animator(Game* g, GameObject* p);

		// Get functions
		std::vector<AnimationNode*> getAnimation() { return animation; }

		// Set functions
		void setAnimation(std::vector<AnimationNode*> a) { animation = a; }

		// Other vector functions
		void deleteAnimation(int pos) { animation.erase(animation.begin() + pos); }
		void appendAnimation(AnimationNode* node) { animation.push_back(node); }

		// Positional Functions
		void skipStep() { animationPosition = ++animationPosition % animation.size(); }
		void rewindStep() { animationPosition = --animationPosition % animation.size(); }
		void setNode(int newPos) { animationPosition = newPos; lastFrameStamp = engine->getElapsedFrames(); }
		void updateAndRender();
	};
}
