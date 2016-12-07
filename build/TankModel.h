#pragma once
#include "gameentity.h"

class TankModel :
	public GameEntity
{
public:
	TankModel(void);
	~TankModel(void);

	void TankModel::buildTankModel(Ogre::Vector3 position);
	void TankModel::animateTank();
};
