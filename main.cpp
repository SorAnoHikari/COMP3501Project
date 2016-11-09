#include <iostream>
#include <exception>
#include "ogre_application.h"

/* Macro for printing exceptions */
#define PrintException(exception_object)\
    std::cerr << exception_object.what() << std::endl

/* Main function that builds and runs the application */
int main(void){
    COMP3501_project::OgreApplication application;

    try {
        application.Init();
        application.LoadModel("desert.obj", "desert");
        Ogre::SceneNode *scene_node = application.CreateEntity("terrain", "desert", "TerrainMaterial");
		scene_node->setScale(11, 11, 11);
		scene_node->setPosition(10, -20, -5);
		//application.SetupAnimation("CubeEnt1");

		application.InitializeAssets();
		// TODO: Fix terrain loading, currently not showing up
		//application.createScene();
        application.MainLoop();
    }
    catch (std::exception &e){
        PrintException(e);
    }

    return 0;
}
