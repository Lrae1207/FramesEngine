#pragma once
#include "DllBase.hpp"
#include "Engine.hpp"

namespace engine {
	struct ENGINE_API AnimationNode {
		float delayFrames; // delay between this node and the next node in render frames
		LoadableTexture* texture;
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