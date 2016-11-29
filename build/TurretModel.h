#pragma once
#include "gameentity.h"

class TurretModel :
	public GameEntity
{
public:
	TurretModel(void);
	~TurretModel(void);

	void TurretModel::buildTurretModel(Ogre::Vector3 position);
	void TurretModel::animateTurret();
};
