#include "GameEntity.h"


GameEntity::GameEntity(void)
{
	direction = Ogre::Vector3(1, 0, 0);
	currentMovement = Ogre::Vector3(0, 0, 0);
}


GameEntity::~GameEntity(void)
{
}

void GameEntity::SetParts(Ogre::SceneNode** inParts)
{
	entity_parts = inParts;
}

Ogre::SceneNode** GameEntity::GetParts(void)
{
	return entity_parts;
}

Ogre::Vector3 GameEntity::GetDirection(void)
{
	return direction;
}

Ogre::Vector3 GameEntity::GetCurrentMovement(void)
{
	return currentMovement;
}

void GameEntity::SetCurrentMovement(Ogre::Vector3 inVector)
{
	currentMovement = inVector;
}

Ogre::Quaternion GameEntity::GetOrientation(void)
{
	return orientation;
}