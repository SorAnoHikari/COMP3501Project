#pragma once
#include <OgreSceneNode.h>

class GameEntity;

class AnimationServices
{
public:
	AnimationServices(void);
	~AnimationServices(void);

	static void MoveEntity(GameEntity entity);
};

