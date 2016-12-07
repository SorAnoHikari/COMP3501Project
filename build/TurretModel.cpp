#include "TurretModel.h"

TurretModel::TurretModel(void) {

}

TurretModel::~TurretModel() {

}

void TurretModel::buildTurretModel(Ogre::Vector3 position) {
	Ogre::Vector3 turret_size = Ogre::Vector3(20, 20, 20);
	entity_parts[0]->setScale(turret_size);
	entity_parts[0]->setPosition(position);

	entity_parts[1]->setScale(turret_size.x/(turret_size.x*10), (turret_size.y/turret_size.y)/2, turret_size.z/(turret_size.z*10));
	entity_parts[1]->roll(Ogre::Angle(90));
	entity_parts[1]->setPosition(0.5, 0.3, 0);
}

void TurretModel::animateTurret() {
	entity_parts[0]->yaw(Ogre::Radian(Ogre::Math::PI/100));
}