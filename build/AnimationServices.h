#pragma once
#include <OgreSceneNode.h>

class GameEntity;

class AnimationServices
{
	
public:
	AnimationServices(void);
	~AnimationServices(void);

	enum Direction
	{
		Forward,
		Right,
		Up
	};

	static void MoveEntity(GameEntity* entity);
	static void AnimationServices::RotateEntity(GameEntity* entity, Ogre::Quaternion rotation);
};

