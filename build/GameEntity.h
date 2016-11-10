#pragma once
#include <OgreSceneNode.h>

class GameEntity
{
public:
	GameEntity(void);
	~GameEntity(void);

	void GameEntity::SetParts(Ogre::SceneNode** inParts);
	void GameEntity::SetCurrentMovement(Ogre::Vector3 inVector);
	void GameEntity::SetOrientation(Ogre::Quaternion inOrientation);
	Ogre::SceneNode** GameEntity::GetParts(void);

	void GameEntity::SetDirection(Ogre::Vector3 inVector);
	void GameEntity::SetRight(Ogre::Vector3 inVector);
	void GameEntity::SetUp(Ogre::Vector3 inVector);

	Ogre::Vector3 GameEntity::GetDirection(void);
	Ogre::Vector3 GameEntity::GetRight(void);
	Ogre::Vector3 GameEntity::GetUp(void);

	Ogre::Quaternion GameEntity::GetOrientation(void);
	Ogre::Vector3 GameEntity::GetCurrentMovement(void);

	int GameEntity::GetNumOfParts(void);

	// for now, make set/getters later
	Ogre::Vector3 position;
	int max_speed;
protected:
	Ogre::SceneNode** entity_parts;
	Ogre::Vector3 currentMovement;

	Ogre::Vector3 direction;
	Ogre::Vector3 right;
	Ogre::Vector3 up;
	Ogre::Quaternion orientation;
	int num_of_parts;
};

