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
        application.LoadModel("cube.obj", "CubeMesh");
        Ogre::SceneNode *scene_node = application.CreateEntity("CubeEnt1", "CubeMesh", "ShinyTextureMaterial");
        scene_node->scale(0.3, 0.3, 0.3);
        application.SetupAnimation("CubeEnt1");
        application.MainLoop();
    }
    catch (std::exception &e){
        PrintException(e);
    }

    return 0;
}
