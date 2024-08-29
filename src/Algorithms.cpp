#include "pch.h"
#include "Algorithms.hpp"

// TODO: replace pointers with raw classes

engine::ASTARGrid::ASTARGrid(int w, int h, sf::Vector2i begin, sf::Vector2i stop) {
	width = w;
	height = h;

	start.x = begin.x;
	start.y = begin.y;
	start.hcost = 0;
	start.gcost = 0;
	start.fcost = 0;

	end.x = stop.x;
	end.y = stop.y;
	end.hcost = 0;
	end.gcost = 0;
	end.fcost = 0;
}

engine::ASTARGrid::~ASTARGrid() {

}

size_t engine::ASTARGrid::addOpen(ASTARNode* node) {
	if (openNodes.empty()) {
		openNodes.push_back(node);
		return 0;
	}
	int i = openNodes.size() - 1;
	while (i > 0 && *openNodes[i] > *node) {
		if (*openNodes[i] == *node) {
			return -1;
		}
		i--;
	}
	openNodes.insert(openNodes.begin() + i + 1, node);
	return i + 1;
}

size_t engine::ASTARGrid::addClosed(ASTARNode* node) {
	if (closedNodes.empty()) {
		closedNodes.push_back(node);
		return 0;
	}
	int i = closedNodes.size()-1;
	while (i > 0 && *closedNodes[i] > *node) {
		if (*closedNodes[i] == *node) {
			return -1;
		}
		i--;
	}
	closedNodes.insert(closedNodes.begin()+i+1, node);
	return i + 1;
}

void engine::ASTARGrid::setBlockedCoords(std::vector<sf::Vector2i> coords) {
	blockedCoords = coords;

	// Sort by index ascending order
	std::sort(blockedCoords.begin(), blockedCoords.end(), [](sf::Vector2i a, sf::Vector2i b) {
		if (a.y == b.y) { return a.x < b.x; }
		return a.y < b.y;
		});
}

std::vector<engine::ASTARNode*> engine::ASTARGrid::getValidNeighbors(ASTARNode* parent) {
	std::vector<ASTARNode*> neighbors = {};

	// Sorry
	sf::Vector2i position = sf::Vector2i(parent->x,parent->y);
	std::vector<sf::Vector2i> positionsToCheck = {};

	positionsToCheck.push_back(position + sf::Vector2i(1, 0));
	positionsToCheck.push_back(position + sf::Vector2i(-1, 0));
	positionsToCheck.push_back(position + sf::Vector2i(0, 1));
	positionsToCheck.push_back(position + sf::Vector2i(0, -1));
	
	// Adjacent
	for (sf::Vector2i neighborPosition : positionsToCheck) {
		if (!validateCoord(neighborPosition)) {
			continue;
		}
		ASTARNode* neighborNode = new ASTARNode;
		neighborNode->x = neighborPosition.x;
		neighborNode->gcost = parent->gcost + 10;
		neighborNode->y = neighborPosition.y;
		neighborNode->parent = parent;
		neighbors.push_back(neighborNode);
	}

	positionsToCheck = {};

	positionsToCheck.push_back(position + sf::Vector2i(-1, 1));
	positionsToCheck.push_back(position + sf::Vector2i(1, 1));
	positionsToCheck.push_back(position + sf::Vector2i(1, -1));
	positionsToCheck.push_back(position + sf::Vector2i(-1, -1));

	// Corners
	for (sf::Vector2i neighborPosition : positionsToCheck) {
		if (!validateCoord(neighborPosition)) {
			continue;
		}
		ASTARNode* neighborNode = new ASTARNode;
		neighborNode->x = neighborPosition.x;
		neighborNode->gcost = parent->gcost + 14;
		neighborNode->y = neighborPosition.y;
		neighborNode->parent = parent;
		neighbors.push_back(neighborNode);
	}

	return neighbors;
}

bool engine::ASTARGrid::validateCoord(sf::Vector2i coord) {
	if (coord.x < 0 || coord.x > width-1 || coord.y < 0 || coord.y > height - 1) {
		return false;
	}

	// If node is blocked
	if (std::find(blockedCoords.begin(),blockedCoords.end(),coord) != blockedCoords.end()) {
		return false;
	}

	for (ASTARNode* node : closedNodes) {
		// If coord is found
		if (node->x == coord.x && node->y == coord.y) {
			return false;
		}
	}
	return true;
}

void engine::ASTARGrid::calculateCost(ASTARNode* node) {
	node->gcost = getDistance(node, &start);
	node->hcost = getDistance(node, &end);

	node->fcost = node->gcost + node->hcost;
}

int engine::ASTARGrid::getDistance(ASTARNode* a, ASTARNode* b) {
	sf::Vector2i diff = sf::Vector2i(a->x, a->y) - sf::Vector2i(b->x, b->y);
	diff.x = abs(diff.x);
	diff.y = abs(diff.y);

	if (diff.x > diff.y) {
		return 14 * diff.y + 10 * (diff.x - diff.y);
	}
	return 14 * diff.x + 10 * (diff.y - diff.x);
}

void engine::ASTARGrid::findPath() {
	openNodes = {};
	closedNodes = {};
	solution = {};

	ASTARNode* startNode = new ASTARNode;
	*startNode = start;
	openNodes.push_back(startNode);

	ASTARNode* currentNode;

	while (!openNodes.empty()) {
		currentNode = openNodes[0];
		for (int i = 0; i < openNodes.size(); ++i) {
			ASTARNode* node = openNodes[i];
			if (node->fcost < currentNode->fcost) {
				currentNode = node;
			}
		}

		openNodes.erase(std::find(openNodes.begin(), openNodes.end(), currentNode));
		addClosed(currentNode);

		// Maybe wont work?
		if ((*currentNode) == end) {
			traceBack(startNode,currentNode);
			break;
		}

		std::vector<ASTARNode*> neighborNodes = getValidNeighbors(currentNode);

		for (ASTARNode* neighbor : neighborNodes) {
			int neighborMoveCost = currentNode->gcost + getDistance(currentNode, neighbor);

			// neighbor not in openNodes
			bool inOpenNodes = duplicateInOpen(neighbor);
			if (neighborMoveCost < neighbor->gcost || !inOpenNodes) {
				calculateCost(neighbor);
				if (!inOpenNodes) {
					addOpen(neighbor);
				}
			}
			else {
				delete neighbor;
			}
		}
	}

	// Release memory
	for (auto it = openNodes.begin(); it != openNodes.end(); it=openNodes.erase(it)) {
		delete *it;
	}
	for (auto it = closedNodes.begin(); it != closedNodes.end(); it = closedNodes.erase(it)) {
		delete* it;
	}
}

bool engine::ASTARGrid::duplicateInOpen(ASTARNode* node) {
	for (auto it = openNodes.begin(); it != openNodes.end(); ++it) {
		if (**it == *node) {
			return true;
		}
	}
	return false;
}

void engine::ASTARGrid::traceBack(ASTARNode* start, ASTARNode* end) {
	std::vector<ASTARNode> path = {};

	ASTARNode* currentNode = end;

	while ((*currentNode) != *start) {
		path.push_back(*currentNode);
		if (currentNode->parent) {
			currentNode = currentNode->parent;
		}
	}

	path.push_back(*start);
	std::reverse(path.begin(), path.end());
	solution = path;
}