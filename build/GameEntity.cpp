#include "GameEntity.h"


GameEntity::GameEntity(void)
{
	forwardDirection = Ogre::Vector3(1, 0, 0);
	rightDirection = Ogre::Vector3(0, 0, 1);
	upDirection = Ogre::Vector3(0, 1, 0);
	position = Ogre::Vector3(0, 0, 0);
	currentMovement = Ogre::Vector3(0, 0, 0);
	num_of_parts = 0;
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::ToggleVisibility(bool isVisible)
{
	for (int i = 0; i < num_of_parts; i++)
	{
		entity_parts[i]->setVisible(isVisible);
	}
}

void GameEntity::SetParts(Ogre::SceneNode** inParts)
{
	entity_parts = inParts;
}

Ogre::SceneNode** GameEntity::GetParts(void)
{
	return entity_parts;
}

Ogre::Vector3 GameEntity::GetForward(void)
{
	return forwardDirection;
}

Ogre::Vector3 GameEntity::GetRight(void)
{
	return rightDirection;
}

Ogre::Vector3 GameEntity::GetUp(void)
{
	return upDirection;
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

void GameEntity::SetNumOfParts(int inNum)
{
	num_of_parts = inNum;
}

void GameEntity::SetForward(Ogre::Vector3 inVector)
{
	forwardDirection = inVector;
}

void GameEntity::SetRight(Ogre::Vector3 inVector)
{
	rightDirection = inVector;
}

void GameEntity::SetUp(Ogre::Vector3 inVector)
{
	upDirection = inVector;
}

int GameEntity::GetRadius(void)
{
	return radius;
}

void GameEntity::SetRadius(int inRadius)
{
	radius = inRadius;
}