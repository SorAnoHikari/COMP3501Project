#pragma once
#include "gameentity.h"
class MissileModel :
	public GameEntity
{
public:
	MissileModel(void);
	~MissileModel(void);

	bool isActive;
private:
};

