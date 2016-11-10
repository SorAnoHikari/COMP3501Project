#include "HelicopterModel.h"


HelicopterModel::HelicopterModel(void)
{
	max_speed = 3;
}


HelicopterModel::~HelicopterModel(void)
{
}

void HelicopterModel::initializeHelicopter_OgreSceneGraph()
{
	float rotorblade_length = 20;
	float topBody_length = 12;
	float tail_length = 4;
	float tail_rotorblade_length = 5;
	#pragma region Scene Graph Transformations
	/* Using Ogre Scene Graph's methods to transform the parts */
	entity_parts[1]->setScale(Ogre::Vector3(0.1, rotorblade_length, 0.1));
	entity_parts[1]->pitch(Ogre::Degree(90));
	entity_parts[1]->setPosition(Ogre::Vector3(0, -0.5, 0));

	entity_parts[2]->setScale(Ogre::Vector3(topBody_length, 1, 2));
	entity_parts[2]->setPosition(Ogre::Vector3(0, -1, 0));

	entity_parts[3]->setScale(Ogre::Vector3(topBody_length + 2, 1, 2));
	entity_parts[3]->setPosition(Ogre::Vector3(1, -2, 0));

	entity_parts[4]->setScale(Ogre::Vector3(0.5, tail_length, 0.5));
	entity_parts[4]->roll(Ogre::Degree(90));
	entity_parts[4]->setPosition(Ogre::Vector3(-1*(tail_length/2 + topBody_length/2), -1.5, 0));

	entity_parts[5]->setScale(Ogre::Vector3(0.1, tail_rotorblade_length, 0.1));
	entity_parts[5]->setPosition(Ogre::Vector3(-1*(tail_length-0.1 + topBody_length/2), -1.5, 0));

	forwardDirection = Ogre::Vector3(1, 0, 0);
	num_of_parts = 7;
	#pragma endregion 
}

void HelicopterModel::animateHelicopter(int timer)
{
	entity_parts[1]->roll(Ogre::Radian(1));
	entity_parts[5]->pitch(Ogre::Radian(1));
}

Ogre::Vector3 HelicopterModel::GetThirdPersonCameraPosition(void)
{
	return entity_parts[6]->_getDerivedPosition();
}

MissileModel* HelicopterModel::GetMissile(void)
{
	return missile;
}
void HelicopterModel::SetMissile(MissileModel* inMissile)
{
	missile = inMissile;
}

void HelicopterModel::FireMissile(void)
{
	missile->position = position;
	missile->GetParts()[0]->setPosition(position);
	missile->GetParts()[0]->setVisible(true);
	missile->SetForward(forwardDirection);
	missile->SetCurrentMovement(forwardDirection*2);
	missile->isActive = true;
}

void HelicopterModel::DeactivateMissile(void)
{
	missile->GetParts()[0]->setVisible(false);
	missile->SetCurrentMovement(Ogre::Vector3(0, 0, 0));
	missile->isActive = false;
}

bool HelicopterModel::IsMissileActive(void)
{
	return missile->isActive;
}