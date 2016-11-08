#pragma once
#include "gameentity.h"
class HelicopterModel :
	public GameEntity
{
public:
	HelicopterModel(void);
	~HelicopterModel(void);
	void HelicopterModel::initializeHelicopter_OgreSceneGraph();
	void HelicopterModel::setHelicopter(Ogre::SceneNode* inHelicopter_[]);
	void HelicopterModel::animateHelicopter(int timer);
};

