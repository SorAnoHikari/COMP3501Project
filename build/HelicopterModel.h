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

	Ogre::String HelicopterModel::GetControlPointsName(void);
	Ogre::String HelicopterModel::GetSplineParticleName(void);
	
	bool IsHomingMissileActive;
	Ogre::Vector3 *control_points;
	int HelicopterModel::GetNumberOfControlPoints(void);
	void HelicopterModel::FireHomingMissile(void);
private:
	MissileModel* missile;
	int num_control_points;
	Ogre::String control_point_name;
	Ogre::String spline_particle_name;
};

