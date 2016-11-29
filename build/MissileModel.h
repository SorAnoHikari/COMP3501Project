#pragma once
#include "gameentity.h"
class MissileModel :
	public GameEntity
{
public:
	MissileModel(void);
	~MissileModel(void);

	void buildMissileModel();

	bool isActive;
private:
};