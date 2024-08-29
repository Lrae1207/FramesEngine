#pragma once
#include "DllBase.hpp"
#include <vector>
#include <algorithm>

namespace engine {
	// Returns index of the T found via binary search
	template<typename T>
	int binarySearch(typename std::vector<T>::iterator lowerBound, typename std::vector<T>::iterator upperBound, T value) {
		std::vector<T> list(lowerBound, upperBound);
		if (list.size() == 1) {
			if (list[0] == value) {
				return 0;
			}
			return -1;
		}
		size_t middle = list.size() / 2;

		auto eval = (list[middle]);
		size_t index;
		if (eval > (value)) { // value is below the middle
			index = binarySearch<T>(list.begin(), list.begin() + middle, value);
			return index;
		}
		if (eval == (value)) { // value is equivalent
			return middle;
		}
		// value is above the middle
		index = binarySearch<T>(list.begin() + middle, list.end(), value);
		if (index == -1) {
			return -1;
		}
		return index + middle;
	}

	template<typename T>
	int linearSearch(typename std::vector<T>::iterator lowerBound, typename std::vector<T>::iterator upperBound, T value) {
		std::vector<T> list(lowerBound, upperBound);
		if (list.empty()) {
			return -1;
		}

		for (auto it = lowerBound; it < upperBound; ++it) {
			if (*it == value) {
				return it - lowerBound;
			}
		}
		return -1;
	}

	// I LOVE SEBASTIAN LAGUE
	struct ENGINE_API ASTARNode {
		ASTARNode* parent;
		int x = -1;
		int y = -1;
		int hcost = -1; // -1 indicates undefined
		int gcost = -1; // -1 indicates undefined
		int fcost = -1; // -1 indicates undefined

		friend bool operator<(const ASTARNode& a, const ASTARNode& b) {
			if (a.y == b.y) {
				return a.x < b.x;
			}
			return a.y < b.y;
		}
		friend bool operator>(const ASTARNode& a, const ASTARNode& b) {
			if (a.y == b.y) {
				return a.x > b.x;
			}
			return a.y > b.y;
		}
		friend bool operator==(const ASTARNode& a, const ASTARNode& b) {
			return a.x == b.x && a.y == b.y;
		}
		friend bool operator!=(const ASTARNode& a, const ASTARNode& b) {
			return a.x != b.x || a.y != b.y;
		}
	};

	// Usually corners are 14 and sides are 10
	class ENGINE_API ASTARGrid {
	private:
		std::vector<sf::Vector2i> blockedCoords;
	public:
		std::vector<ASTARNode*> openNodes; // keep this sorted by ascending f-cost
		std::vector<ASTARNode*> closedNodes; // keep sorted by ascending f-cost
		std::vector<ASTARNode> solution = {};

		int width;
		int height;

		ASTARNode start;
		ASTARNode end;

		ASTARGrid(int w, int h, sf::Vector2i begin, sf::Vector2i stop);
		~ASTARGrid();

		size_t addOpen(ASTARNode* node);
		size_t addClosed(ASTARNode* node);

		void setBlockedCoords(std::vector<sf::Vector2i> coords);

		std::vector<ASTARNode*> getValidNeighbors(ASTARNode* parent);
		bool validateCoord(sf::Vector2i coord);

		void calculateCost(ASTARNode* node);
		int getDistance(ASTARNode* a, ASTARNode* b);
		bool duplicateInOpen(ASTARNode* node);

		void findPath();
		void traceBack(ASTARNode* start, ASTARNode* end);
	};
};