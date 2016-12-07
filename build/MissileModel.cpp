#include "MissileModel.h"


MissileModel::MissileModel(void)
{
	radius = 1;
	isActive = false;
}


MissileModel::~MissileModel(void)
{
}

void MissileModel::buildMissileModel(void) 
{
	Ogre::Vector3 missile_body = Ogre::Vector3(0.5, 2, 0.5);
	float top_height = missile_body.y/8;

	entity_parts[0]->setScale(missile_body);
	entity_parts[0]->roll(Ogre::Angle(-90));

	entity_parts[1]->setScale(missile_body.x*4, top_height, missile_body.z*4);
	entity_parts[1]->setPosition(0, top_height*5, 0);
	
	entity_parts[2]->setScale(1, missile_body.y/4, 1);
	entity_parts[2]->setPosition(missile_body.x + entity_parts[2]->getScale().x/4, -missile_body.y/4, 0);
	
	entity_parts[3]->setScale(1, missile_body.y/4, 1);
	entity_parts[3]->setPosition(-entity_parts[2]->getPosition().x, -missile_body.y/4, 0);

	entity_parts[4]->setScale(1, missile_body.y/4, 1);
	entity_parts[4]->setPosition(0, -missile_body.y/4, missile_body.z + entity_parts[4]->getScale().z/2);

	entity_parts[5]->setScale(1, missile_body.y/4, 1);
	entity_parts[5]->setPosition(0, -missile_body.y/4, -entity_parts[4]->getPosition().z);

	entity_parts[6]->setScale(1, 0.5, 1);
	entity_parts[6]->setPosition(0, -top_height*4, 0);
}