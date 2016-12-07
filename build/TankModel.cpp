#include "TankModel.h"

TankModel::TankModel(void) {

}

TankModel::~TankModel() {

}

void TankModel::buildTankModel(Ogre::Vector3 position) {
	Ogre::Vector3 tank_size = Ogre::Vector3(10, 5, 5);
	entity_parts[0]->setScale(tank_size);

	entity_parts[1]->setInheritScale(false);
	entity_parts[1]->setScale(2, 2, 2);
	entity_parts[1]->setPosition(0, 0.6, 0);

	entity_parts[2]->setInheritScale(false);
	entity_parts[2]->setScale(1, 3, 1);
	entity_parts[2]->roll(Ogre::Angle(90));
	entity_parts[2]->setPosition(0.8, 0.3, 0);
}

void TankModel::animateTank() {
}