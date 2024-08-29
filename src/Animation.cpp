#include "pch.h"
#include "Animation.hpp"

engine::Animator::Animator(Game* g, GameObject* p) {
	engine = g;
	shape = p->getShapeComponent();
}

void engine::Animator::updateAndRender() {
	if (engine->getElapsedFrames() - lastFrameStamp > animation[animationPosition]->delayFrames) {
		animationPosition++;
		if (animationPosition > animation.size()) {
			animationPosition %= animation.size();
		}
		AnimationNode* currentNode = animation[animationPosition];
		shape->texture = currentNode->texture;
		shape->texture->setOffset(currentNode->animationOffset);
	}
}