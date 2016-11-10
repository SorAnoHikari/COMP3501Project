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
	entity->GetParts()[0]->translate(entityTranslation);
	entity->position += entityTranslation;
}

void AnimationServices::RotateEntity(GameEntity* entity, Ogre::Quaternion rotation)
{
	entity->SetForward(rotation * entity->GetForward());
	entity->SetRight(rotation * entity->GetRight());
	entity->SetUp(rotation * entity->GetUp());
	entity->GetParts()[0]->rotate(rotation, Ogre::Node::TS_PARENT);
}