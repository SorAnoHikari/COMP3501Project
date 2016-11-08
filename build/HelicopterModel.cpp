#include "HelicopterModel.h"


HelicopterModel::HelicopterModel(void)
{
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

	direction = Ogre::Vector3(1, 0, 0);
	#pragma endregion 
}

void HelicopterModel::animateHelicopter(int timer)
{
	entity_parts[1]->roll(Ogre::Radian(1));
	entity_parts[5]->pitch(Ogre::Radian(1));

	/*float zTranslationValue = Ogre::Math::Sin(timer/20)/5;
	for (int i = 0; i < 6; i++)
	{
		helicopter_[i]->translate(Ogre::Vector3(0.0, 0, zTranslationValue));
	}*/
}
