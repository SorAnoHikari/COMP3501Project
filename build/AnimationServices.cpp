#include "AnimationServices.h"
#include "GameEntity.h"


AnimationServices::AnimationServices(void)
{
}


AnimationServices::~AnimationServices(void)
{
}

void AnimationServices::MoveEntity(GameEntity entity)
{
	Ogre::Vector3 entityTranslation = entity.GetCurrentMovement();
	Ogre::Quaternion entityOrientation = entity.GetOrientation();
	for (int i = 0; i < 6; i++)
	{
		// TODO: add Null or non 0 check later
		
		entity.GetParts()[i]->translate(entityTranslation);
		entity.GetParts()[i]->rotate(entityOrientation);
	}
}