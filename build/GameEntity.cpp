#include "GameEntity.h"


GameEntity::GameEntity(void)
{
	direction = Ogre::Vector3(1, 0, 0);
	right = Ogre::Vector3(0, 0, 1);
	up = Ogre::Vector3(0, 1, 0);
	position = Ogre::Vector3(0, 0, 0);
	currentMovement = Ogre::Vector3(0, 0, 0);
	num_of_parts = 0;
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

Ogre::Vector3 GameEntity::GetRight(void)
{
	return right;
}

Ogre::Vector3 GameEntity::GetUp(void)
{
	return up;
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

void GameEntity::SetOrientation(Ogre::Quaternion inOrientation)
{
	orientation = inOrientation;
}

int GameEntity::GetNumOfParts(void)
{
	return num_of_parts;
}

void GameEntity::SetDirection(Ogre::Vector3 inVector)
{
	direction = inVector;
}

void GameEntity::SetRight(Ogre::Vector3 inVector)
{
	right = inVector;
}

void GameEntity::SetUp(Ogre::Vector3 inVector)
{
	up = inVector;
}