#pragma once
#include <OgreSceneNode.h>

class GameEntity
{
public:
	GameEntity(void);
	~GameEntity(void);

	void GameEntity::SetParts(Ogre::SceneNode** inParts);
	void GameEntity::SetCurrentMovement(Ogre::Vector3 inVector);
	Ogre::SceneNode** GameEntity::GetParts(void);

	Ogre::Vector3 GameEntity::GetDirection(void);
	Ogre::Quaternion GameEntity::GetOrientation(void);
	Ogre::Vector3 GameEntity::GetCurrentMovement(void);
protected:
	Ogre::SceneNode** entity_parts;
	Ogre::Vector3 currentMovement;
	Ogre::Vector3 position;

	Ogre::Vector3 direction;
	Ogre::Quaternion orientation;
};

