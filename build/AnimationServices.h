#pragma once
#include <OgreSceneNode.h>

class MissileModel;
class GameEntity;

class AnimationServices
{
	
public:
	AnimationServices(void);
	~AnimationServices(void);

	enum Direction
	{
		Forward,
		Right,
		Up
	};

	static void MoveEntity(GameEntity* entity);
	static void AnimationServices::RotateEntity(GameEntity* entity, Ogre::Quaternion rotation);
	static GameEntity* AnimationServices::DetectMissileCollision(MissileModel* missile, GameEntity** objects);
	static void AnimationServices::CreateControlPoints(Ogre::String control_points_name, int num_control_points, Ogre::String material_name, Ogre::Vector3 startingPoint, Ogre::Vector3 endingPoint, Ogre::Vector3 *control_point, Ogre::SceneManager* scene_manager);
};

