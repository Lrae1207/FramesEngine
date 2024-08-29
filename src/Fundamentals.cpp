#pragma once
#include "pch.h"
#include "Fundamentals.hpp"



//  Returns a normalized vector (x and y divided by magnitude)
sf::Vector2f engine::vmath::normalizeVector(sf::Vector2f vector) {
	float magnitude = sqrtf(vector.x * vector.x + vector.y * vector.y);

	// This means vector is <0,0>
	if (magnitude == 0) {
		return vector;
	}
	vector = sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
	return vector;
}

float engine::vmath::getMagnitude(sf::Vector2f vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y);;
}

float engine::vmath::dotProduct(sf::Vector2f a, sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}

float engine::vmath::distanceAlongProjection(sf::Vector2f a, sf::Vector2f b) {
	return dotProduct(a, b);
}

sf::Vector2f ENGINE_API engine::vmath::rotateAroundOrigin(sf::Vector2f point, sf::Vector2f origin, float rotationDegrees) {
	float sin = sinf(DEG2RAD(rotationDegrees));
	float cos = cosf(DEG2RAD(rotationDegrees));
	point -= origin;

	float xnew = point.x * cos - point.y * sin;
	float ynew = point.x * sin + point.y * cos;

	return (sf::Vector2f(xnew, ynew) + origin);
}

float engine::vmath::getDistance(sf::Vector2f v1, sf::Vector2f v2) {
	return getMagnitude(subtractVectors(v1, v2));
}

sf::Vector2f engine::vmath::addVectors(sf::Vector2f v1, sf::Vector2f v2) {
	return sf::Vector2f(v1.x + v2.x, v1.y + v2.y);
}

sf::Vector2f engine::vmath::rotateByDegrees(sf::Vector2f vector, float degrees) {
	sf::Vector2f returnVector;
	degrees = DEG2RAD(degrees);
	returnVector.x = vector.x * cosf(degrees) - vector.y * sinf(degrees);
	returnVector.y = vector.x * sinf(degrees) + vector.y * cosf(degrees);
	return returnVector;
}

/* Returns v1 - v2 */
sf::Vector2f engine::vmath::subtractVectors(sf::Vector2f v1, sf::Vector2f v2) {
	return sf::Vector2f(v1.x - v2.x, v1.y - v2.y);
}

sf::Vector2f engine::vmath::multiplyVector(sf::Vector2f v1, float i) {
	return sf::Vector2f(v1.x * i, v1.y * i);
}

sf::Vector2f engine::vmath::divideVector(sf::Vector2f v1, float i) {
	return sf::Vector2f(v1.x / i, v1.y / i);
}

sf::Vector2f engine::vmath::utof(sf::Vector2u v1) {
	return sf::Vector2f(v1.x, v1.y);
};

engine::Rect engine::rotateRect(Rect r, sf::Vector2f origin, float degrees) {
	sf::Vector2f topLeft = sf::Vector2f(r.left, r.top);
	sf::Vector2f botRight = sf::Vector2f(r.right, r.bottom);

	topLeft = vmath::rotateAroundOrigin(topLeft, origin, degrees);
	botRight = vmath::rotateAroundOrigin(botRight, origin, degrees);

	r.top = topLeft.y;
	r.left = topLeft.x;
	r.bottom = botRight.y;
	r.right = botRight.x;

	return r;
}

engine::Rect engine::addVector(Rect r, sf::Vector2f v) {
	r.left += v.x;
	r.right += v.x;
	r.top += v.y;
	r.bottom += v.y;
	return r;
}

engine::floatPolygon engine::rectToPolygon(Rect r) {
	return { sf::Vector2f(r.left,r.top), sf::Vector2f(r.right,r.top), sf::Vector2f(r.right,r.bottom), sf::Vector2f(r.left,r.bottom) };
}

engine::Rect engine::vectorToRect(sf::Vector2f v) {
	engine::Rect r;
	r.top = 0;
	r.left = 0;
	r.right = v.x;
	r.bottom = v.y;
	return r;
}