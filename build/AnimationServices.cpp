#include "AnimationServices.h"
#include "GameEntity.h"


AnimationServices::AnimationServices(void)
{
}


AnimationServices::~AnimationServices(void)
{
}

void AnimationServices::MoveEntity(GameEntity* entity)
{
	Ogre::Vector3 entityTranslation = entity->GetCurrentMovement();
	for (int i = 0; i < entity->GetNumOfParts(); i++)
	{
		// TODO: add Null or non 0 check later
		
		entity->GetParts()[i]->translate(entityTranslation);
	}
	entity->position += entityTranslation;
}

void AnimationServices::RotateEntity(GameEntity* entity, Ogre::Quaternion rotation)
{
	for (int i = 0; i < entity->GetNumOfParts(); i++)
	{
		entity->GetParts()[i]->rotate(rotation);
	}
	entity->SetDirection(rotation * entity->GetDirection());
	entity->SetRight(rotation * entity->GetRight());
	entity->SetUp(rotation * entity->GetUp());
}