#include "AnimationServices.h"
#include "GameEntity.h"
#include "../ogre_application.h"


AnimationServices::AnimationServices(void)
{
}


AnimationServices::~AnimationServices(void)
{
}

void AnimationServices::MoveEntity(GameEntity* entity)
{
	Ogre::Vector3 entityTranslation = entity->GetCurrentMovement();
	entity->GetParts()[0]->translate(entityTranslation);
	entity->position += entityTranslation;
}

void AnimationServices::RotateEntity(GameEntity* entity, Ogre::Quaternion rotation)
{
	entity->SetForward(rotation * entity->GetForward());
	entity->SetRight(rotation * entity->GetRight());
	entity->SetUp(rotation * entity->GetUp());
	entity->GetParts()[0]->rotate(rotation, Ogre::Node::TS_PARENT);
}

/*
 *	Basic sphere-sphere collision detection for missiles 
 *	Maybe implement box collision for better effect later.
 */
GameEntity* AnimationServices::DetectMissileCollision(MissileModel* missile, GameEntity** objects)
{
	for (int i = 0; i < 30; i++)
	{
		if (missile->position.distance(objects[i]->position) <= missile->GetRadius() + objects[i]->GetRadius()) {
			return objects[i];
		}
	}
	return nullptr;
}

void AnimationServices::CreateControlPoints(Ogre::String control_points_name, int num_control_points, Ogre::String material_name, Ogre::Vector3 startingPoint, Ogre::Vector3 endingPoint, Ogre::Vector3 *control_point, Ogre::SceneManager* scene_manager){

	try {
		/* Allocate memory for control points */
		control_point = new Ogre::Vector3[num_control_points];

		/* Create control points of a piecewise spline */
		/* We store the control points in groups of 4 */
		/* Each group represents the control points (p0, p1, p2, p3) of a cubic Bezier curve */
		/* To ensure C1 continuity, we constrain the first and second point of each curve according to the previous curve */
        
		Ogre::Vector3 normalizedTrajectory = (startingPoint - endingPoint).normalisedCopy();
		float distance_to_increment = startingPoint.distance(endingPoint)/num_control_points;

		// Initialize the first two control points to fixed values */
		control_point[0] = startingPoint;
		control_point[1] = startingPoint - 2 * normalizedTrajectory;

		// Create remaining points
		for (int i = 2; i < num_control_points-2; i++){
			// Check if we have the first or second point of a curve
			// Then we need to constrain the points
			if (i % 4 == 0){
				// Constrain the first point of the curve
				// p3 = q0, where the previous curve is (p0, p1, p2, p3) and the current curve is (q0, q1, q2, q3)
				// p3 is at position -1 from the current point q0
				control_point[i] = control_point[i - 1];
			} else if (i % 4 == 1){
				// Constrain the second point of the curve
				// q1 = 2*p3 Ep2
				// p3 is at position -1 and we add another -1 since we are at i%4 == 1 (not i%4 == 0)
				// p2 is at position -2 and we add another -1 since we are at i%4 == 1 (not i%4 == 0)
				control_point[i] = 2.0*control_point[i -2] - control_point[i - 3];
			} else {
				// Other points: we can freely assign random values to them
				// Get 3 random numbers
				float u, v, w;
				u = rand() % 3 + 1;
				v = rand() % 3 + 1;
				w = rand() % 3 + 1;
				// Define control points based on u, v, and w and scale by the control point index
				control_point[i] = control_point[i] = control_point[i-1] + distance_to_increment*normalizedTrajectory; + Ogre::Vector3(u, v, w);
				//control_point[i] = Ogre::Vector3(u*3.0*(i/4 + 1), v*3.0*(i/4+1), 0.0); // Easier to visualize with the control points on the screen
			}
		}

		control_point[num_control_points-1] = startingPoint + 2 * normalizedTrajectory;
		control_point[num_control_points] = endingPoint;

		/* Add control points to the material's shader */
		/* Translate the array of Ogre::Vector3 to an accepted format */
		float *shader_data;
		shader_data = new float[num_control_points*4];
		for (int i = 0; i < num_control_points; i++){
			shader_data[i*4] = control_point[i].x;
			shader_data[i*4 + 1] = control_point[i].y;
			shader_data[i*4 + 2] = control_point[i].z;
			shader_data[i*4 + 3] = 0.0;
		}

		/* Add array as a parameter to the shader of the specified material */
		Ogre::MaterialPtr mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(material_name));
		mat->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("control_point", shader_data, num_control_points, 4);

		/* Also create a mesh out of the control points, so that we can render them, if needed */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(control_points_name);
        object->setDynamic(false);
		object->begin("", Ogre::RenderOperation::OT_POINT_LIST);
		for (int i = 0; i < num_control_points; i++){
			object->position(control_point[i]);
			// Color allows us to keep track of control point ordering
			object->colour(1.0 - ((float) i)/((float)num_control_points), 0.0, ((float) i)/((float)num_control_points));
		}
		object->end();
        object->convertToMesh(control_points_name);

		/* Free memory we used to store control points temporarily */
		//delete [] control_point;
	}
    catch (Ogre::Exception &e){
        throw std::string("Ogre::Exception: ") + std::string(e.what());
    }
    catch(std::exception &e){
        throw std::string("std::Exception: ") + std::string(e.what());
    }	
}