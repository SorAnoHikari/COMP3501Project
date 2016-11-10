#pragma once
#include "gameentity.h"
#include "MissileModel.h"

class HelicopterModel :
	public GameEntity
{
public:
	HelicopterModel(void);
	~HelicopterModel(void);

	void HelicopterModel::initializeHelicopter_OgreSceneGraph();
	void HelicopterModel::setHelicopter(Ogre::SceneNode* inHelicopter_[]);
	void HelicopterModel::animateHelicopter(int timer);
	Ogre::Vector3 HelicopterModel::GetThirdPersonCameraPosition(void);

	MissileModel* HelicopterModel::GetMissile(void);
	void HelicopterModel::SetMissile(MissileModel* inMissile);
	void HelicopterModel::FireMissile(void);
	void HelicopterModel::DeactivateMissile(void);
	bool HelicopterModel::IsMissileActive(void);
private:
	MissileModel* missile;
};

