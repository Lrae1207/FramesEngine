#pragma once
#include "DllBase.hpp"

#define PI   3.14159265358979323846264338327950288
#define DEG2RAD(angleInDegrees) ((angleInDegrees) * PI / 180.0)
#define RAD2DEG(angleInRadians) ((angleInRadians) * 180.0 / PI)

namespace engine {
	class GameObject;

	namespace vmath {
		sf::Vector2f ENGINE_API normalizeVector(sf::Vector2f vector);
		float ENGINE_API getMagnitude(sf::Vector2f vector);

		float ENGINE_API getDistance(sf::Vector2f v1, sf::Vector2f v2);

		sf::Vector2f ENGINE_API rotateByDegrees(sf::Vector2f vector, float degrees);

		sf::Vector2f ENGINE_API addVectors(sf::Vector2f v1, sf::Vector2f v2);
		sf::Vector2f ENGINE_API subtractVectors(sf::Vector2f v1, sf::Vector2f v2);

		float ENGINE_API dotProduct(sf::Vector2f a, sf::Vector2f b);
		float ENGINE_API distanceAlongProjection(sf::Vector2f a, sf::Vector2f b);
		sf::Vector2f ENGINE_API rotateAroundOrigin(sf::Vector2f point, sf::Vector2f origin, float rotationDegrees);

		sf::Vector2f ENGINE_API multiplyVector(sf::Vector2f v1, float i);
		sf::Vector2f ENGINE_API divideVector(sf::Vector2f v1, float i);

		sf::Vector2f ENGINE_API utof(sf::Vector2u v1);
	};

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

	struct ENGINE_API StrippedTransform {
		sf::Vector2f position = sf::Vector2f(0, 0);
		float rotation = 0;
		sf::Vector2f origin = sf::Vector2f(0, 0);
		sf::Vector2f scale = sf::Vector2f(1, 1);
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
		GameObject* parent;
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

	/* Holds 4 values*/
	struct Rect {
		float top, left, bottom, right;
	};
	Rect ENGINE_API rotateRect(Rect r, sf::Vector2f origin, float degrees);
	Rect ENGINE_API addVector(Rect r, sf::Vector2f v);
	floatPolygon ENGINE_API rectToPolygon(Rect r);
	Rect ENGINE_API vectorToRect(sf::Vector2f v);
};