#include "ogre_application.h"
#include "bin/path_config.h"
#include "AnimationServices.h"
#include "build/TurretModel.h"
#include "build/TankModel.h"

namespace COMP3501_project {

/* Some configuration constants */
/* They are written here as global variables, but ideally they should be loaded from a configuration file */

/* Initialization */
const Ogre::String config_filename_g = "";
const Ogre::String plugins_filename_g = "";
const Ogre::String log_filename_g = "Ogre.log";

/* Main window settings */
const Ogre::String window_title_g = "Demo";
const Ogre::String custom_window_capacities_g = "";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;

/* Viewport and camera settings */
float viewport_width_g = 0.95f;
float viewport_height_g = 0.95f;
float viewport_left_g = (1.0f - viewport_width_g) * 0.5f;
float viewport_top_g = (1.0f - viewport_height_g) * 0.5f;
unsigned short viewport_z_order_g = 100;
const Ogre::ColourValue viewport_background_color_g(0.0, 0.0, 0.0);
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 2400.0;
Ogre::Vector3 camera_look_at_g(0.0, 0.0, 0.0);
Ogre::Vector3 camera_up_g(0.0, 1.0, 0.0);

/* Materials */
const Ogre::String material_directory_g = MATERIAL_DIRECTORY;


OgreApplication::OgreApplication(void){

    /* Don't do work in the constructor, leave it for the Init() function */
}


void OgreApplication::Init(void){

    /* Set default values for the variables */
    animating_ = false;
    space_down_ = false;
    animation_state_ = NULL;
    input_manager_ = NULL;
    keyboard_ = NULL;
    mouse_ = NULL;
	timer_ = 0;

	camera_thirdperson_offset = Ogre::Vector3(-30, 20.5, 0);
	isInThirdPerson = true;

	NUMBER_OF_ENEMIES = 20;
	enemies_ = new GameEntity*[NUMBER_OF_ENEMIES];

    /* Run all initialization steps */
    InitRootNode();
    InitPlugins();
    InitRenderSystem();
    InitWindow();
    InitViewport();
    InitEvents();
    InitOIS();
    LoadMaterials();
}


void OgreApplication::InitRootNode(void){

    try {
        
        /* We need to have an Ogre root to be able to access all Ogre functions */
        ogre_root_ = std::auto_ptr<Ogre::Root>(new Ogre::Root(config_filename_g, plugins_filename_g, log_filename_g));
        //ogre_root_->showConfigDialog();

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitPlugins(void){

    try {

        /* Load plugin responsible for OpenGL render system */
        Strings plugin_names;
        plugin_names.push_back("RenderSystem_GL");
        
        Strings::iterator iter = plugin_names.begin();
        Strings::iterator iter_end = plugin_names.end();
        for (; iter != iter_end; iter++){
            Ogre::String& plugin_name = (*iter);
            if (OGRE_DEBUG_MODE){
                plugin_name.append("_d");
            }
            ogre_root_->loadPlugin(plugin_name);
        }

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitRenderSystem(void){

    try {

        const Ogre::RenderSystemList& render_system_list = ogre_root_->getAvailableRenderers();
        if (render_system_list.size() == 0)
        {
            throw(OgreAppException(std::string("OgreApp::Exception: Sorry, no rendersystem was found.")));
        }

        Ogre::RenderSystem *render_system = render_system_list.at(0);
        ogre_root_->setRenderSystem(render_system);

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

        
void OgreApplication::InitWindow(void){

    try {

        /* Create main window for the application */
        bool create_window_automatically = false;
        ogre_root_->initialise(create_window_automatically, window_title_g, custom_window_capacities_g);

        Ogre::NameValuePairList params;
        params["FSAA"] = "0";
        params["vsync"] = "true";
        ogre_window_ = ogre_root_->createRenderWindow(window_title_g, window_width_g, window_height_g, window_full_screen_g, &params);

        ogre_window_->setActive(true);
        ogre_window_->setAutoUpdated(false);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitViewport(void){

    try {

        /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create camera object */
        Ogre::Camera* camera = scene_manager->createCamera("MyCamera");
        Ogre::SceneNode* camera_scene_node = root_scene_node->createChildSceneNode("MyCameraNode");
        camera_scene_node->attachObject(camera);

        camera->setNearClipDistance(camera_near_clip_distance_g);
        camera->setFarClipDistance(camera_far_clip_distance_g); 

        camera->setPosition(camera_thirdperson_offset);
        camera->lookAt(camera_look_at_g);
        camera->setFixedYawAxis(true, camera_up_g);

		camera->setNearClipDistance(0.1);

        /* Create viewport */
        Ogre::Viewport *viewport = ogre_window_->addViewport(camera, viewport_z_order_g, viewport_left_g, viewport_top_g, viewport_width_g, viewport_height_g);

        viewport->setAutoUpdated(true);
        viewport->setBackgroundColour(viewport_background_color_g);

        /* Set aspect ratio */
        float ratio = float(viewport->getActualWidth()) / float(viewport->getActualHeight());
        camera->setAspectRatio(ratio);

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitEvents(void){

    try {

        /* Add this object as a FrameListener for render events (see frameRenderingQueued event) */
        ogre_root_->addFrameListener(this);

        /* Add this object as a WindowEventListener to handle the window resize event */
        Ogre::WindowEventUtilities::addWindowEventListener(ogre_window_, this);

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitOIS(void){

    /* Initialize the Object Oriented Input System (OIS) */
    try {

        /* Initialize input manager */
        OIS::ParamList pl; // Parameter list passed to the input manager initialization
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        ogre_window_->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        pl.insert(std::make_pair(std::string("w32_mouse"), 
        std::string("DISCL_FOREGROUND" )));
        pl.insert(std::make_pair(std::string("w32_mouse"), 
        std::string("DISCL_NONEXCLUSIVE")));
        input_manager_ = OIS::InputManager::createInputSystem(pl);

        /*size_t hWnd = 0;
        ogre_window_->getCustomAttribute("WINDOW", &hWnd);
        input_manager_ = OIS::InputManager::createInputSystem(hWnd);*/

        /* Initialize keyboard and mouse */
        keyboard_ = static_cast<OIS::Keyboard*>(input_manager_->createInputObject(OIS::OISKeyboard, false));

        mouse_ = static_cast<OIS::Mouse*>(input_manager_->createInputObject(OIS::OISMouse, false));
        unsigned int width, height, depth;
        int top, left;
        ogre_window_->getMetrics(width, height, depth, left, top);
        const OIS::MouseState &ms = mouse_->getMouseState();
        ms.width = width;
        ms.height = height;

    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateSquare(Ogre::String object_name, Ogre::String material_name)
{
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
	Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	/* Create the 3D object */
    Ogre::ManualObject* object = NULL;
    object = scene_manager->createManualObject(object_name);
    object->setDynamic(false);

    /* Create triangle list for the object */
    object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

	object->position(Ogre::Vector3(-0.5, 0, -0.5));
	object->normal(Ogre::Vector3( 0.0,  1.0,  0.0));
	object->textureCoord(0, 0);

	object->position(Ogre::Vector3(-0.5, 0, 0.5));
	object->normal(Ogre::Vector3( 0.0,  1.0,  0.0));
	object->textureCoord(0, 0);

	object->position(Ogre::Vector3(0.5, 0, 0.5));
	object->normal(Ogre::Vector3( 0.0,  1.0,  0.0));
	object->textureCoord(0, 0);

	object->position(Ogre::Vector3(-0.5, 0, -0.5));
	object->normal(Ogre::Vector3( 0.0,  1.0,  0.0));
	object->textureCoord(0, 0);

	object->position(Ogre::Vector3(0.5, 0, 0.5));
	object->normal(Ogre::Vector3( 0.0,  1.0,  0.0));
	object->textureCoord(0, 0);

	object->position(Ogre::Vector3(0.5, 0, -0.5));
	object->normal(Ogre::Vector3( 0.0,  1.0,  0.0));
	object->textureCoord(0, 0);

	object->end();

	object->convertToMesh(object_name);
}

void OgreApplication::CreateCube(Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create a cube */

		/* Retrieve scene manager and root scene node */
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create triangle list for the object */
        object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Vertices of a cube */
		Ogre::Vector3 v0(-0.5, -0.5,  0.5);
		Ogre::Vector3 v1( 0.5, -0.5,  0.5);
		Ogre::Vector3 v2( 0.5,  0.5,  0.5);
		Ogre::Vector3 v3(-0.5,  0.5,  0.5);
		Ogre::Vector3 v4(-0.5, -0.5, -0.5);
		Ogre::Vector3 v5( 0.5, -0.5, -0.5);
		Ogre::Vector3 v6( 0.5,  0.5, -0.5);
		Ogre::Vector3 v7(-0.5,  0.5, -0.5);

		/* Normal of each face of the cube */
		Ogre::Vector3 n0( 0.0,  0.0,  1.0);
		Ogre::Vector3 n1( 1.0,  0.0,  0.0);
		Ogre::Vector3 n2( 0.0,  0.0, -1.0);
		Ogre::Vector3 n3(-1.0,  0.0,  0.0);
		Ogre::Vector3 n4( 0.0,  1.0,  0.0);
		Ogre::Vector3 n5( 0.0, -1.0,  0.0);

		/* Cube's color */
		Ogre::ColourValue clr0(0.0, 0.0, 1.0);
		Ogre::ColourValue clr1(1.0, 0.0, 1.0);
		Ogre::ColourValue clr2(1.0, 1.0, 1.0);
		Ogre::ColourValue clr3(0.0, 1.0, 0.0);
		Ogre::ColourValue clr4(0.0, 0.0, 1.0);
		Ogre::ColourValue clr5(1.0, 0.0, 0.0);
		Ogre::ColourValue clr6(1.0, 1.0, 0.0);
		Ogre::ColourValue clr7(0.0, 1.0, 0.0);
		
		/* This construction only partially uses shared vertices, so that we can assign appropriate vertex normals
		   to each face */
		/* Each face of the cube is defined by four vertices (with the same normal) and two triangles */
		object->position(v0);
		object->normal(n0);
		object->textureCoord(0, 0);
		object->colour(clr0);

		object->position(v1);
		object->normal(n0);
		object->textureCoord(1, 0);
		object->colour(clr1);

		object->position(v2);
		object->normal(n0);
		object->textureCoord(1, 1);
		object->colour(clr2);

		object->position(v3);
		object->normal(n0);
		object->textureCoord(0, 1);
		object->colour(clr3);
		
		object->position(v1);
		object->normal(n1);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v5);
		object->normal(n1);
		object->textureCoord(1, 0);
		object->colour(clr5);

		object->position(v6);
		object->normal(n1);
		object->textureCoord(1, 1);
		object->colour(clr6);

		object->position(v2);
		object->normal(n1);
		object->textureCoord(0, 1);
		object->colour(clr2);

		object->position(v5);
		object->normal(n2);
		object->textureCoord(0, 0);
		object->colour(clr5);

		object->position(v4);
		object->normal(n2);
		object->textureCoord(1, 0);
		object->colour(clr4);
		
		object->position(v7);
		object->normal(n2);
		object->textureCoord(1, 1);
		object->colour(clr7);

		object->position(v6);
		object->normal(n2);
		object->textureCoord(0, 1);
		object->colour(clr6);

		object->position(v4);
		object->normal(n3);
		object->textureCoord(0, 0);
		object->colour(clr4);

		object->position(v0);
		object->normal(n3);
		object->textureCoord(1, 0);
		object->colour(clr0);

		object->position(v3);
		object->normal(n3);
		object->textureCoord(1, 1);
		object->colour(clr3);

		object->position(v7);
		object->normal(n3);
		object->textureCoord(0, 1);
		object->colour(clr7);

		object->position(v3);
		object->normal(n4);
		object->textureCoord(0, 0);
		object->colour(clr3);

		object->position(v2);
		object->normal(n4);
		object->textureCoord(1, 0);
		object->colour(clr2);

		object->position(v6);
		object->normal(n4);
		object->textureCoord(1, 1);
		object->colour(clr6);

		object->position(v7);
		object->normal(n4);
		object->textureCoord(0, 1);
		object->colour(clr7);

		object->position(v1);
		object->normal(n5);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v0);
		object->normal(n5);
		object->textureCoord(1, 0);
		object->colour(clr0);

		object->position(v4);
		object->normal(n5);
		object->textureCoord(1, 1);
		object->colour(clr4);

		object->position(v5);
		object->normal(n5);
		object->textureCoord(0, 1);
		object->colour(clr5);

		for (int i = 0; i < 6; i++){
			object->triangle(i*4 + 0, i*4 + 1, i*4 + 3);
			object->triangle(i*4 + 1, i*4 + 2, i*4 + 3);
		}
   
		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);

	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateCylinder(Ogre::String object_name, float circle_radius, Ogre::String material_name) {
	try {
		/* Retrieve scene manager and root scene node */
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	
		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		object = scene_manager->createManualObject(object_name);
		object->setDynamic(false);

		/* Create triangle list for the object */
		float const sample = 35;
		int const height = 1;

		Ogre::Vector3 top_circle_centre = Ogre::Vector3(0, height, 0);
		Ogre::Vector3 bottom_circle_centre = -top_circle_centre;
 
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);
 
		int index = 0;
		float t = Ogre::Math::PI/sample;
		for(float theta = t; theta <= Ogre::Math::TWO_PI + t; theta += t) {
			//Build the Top Circle of the cylinder
			object->position(top_circle_centre);
			object->normal(0, 1, 0);
			object->textureCoord(0, height);

			object->position(circle_radius * cos(theta), height, circle_radius * sin(theta)); 
			object->normal(0, 1, 0);
			object->textureCoord(theta/Ogre::Math::TWO_PI, height);

			object->position(circle_radius * cos(theta - Ogre::Math::PI / sample), height, circle_radius * sin(theta - Ogre::Math::PI / sample));
			object->normal(0, 1, 0);
			object->textureCoord((theta - Ogre::Math::PI/sample)/Ogre::Math::TWO_PI, height);

			object->triangle(index, index + 1, index + 2);
			object->triangle(index + 2, index + 1, index);
			index += 3;

			//Build the Body of the cylinder
			object->position(circle_radius * cos(theta), height, circle_radius * sin(theta));
			object->normal(circle_radius * cos(theta), height, circle_radius * sin(theta));
			object->textureCoord(theta/Ogre::Math::TWO_PI, height);

			object->position(circle_radius * cos(theta - Ogre::Math::PI / sample), height, circle_radius * sin(theta - Ogre::Math::PI / sample));
			object->normal(circle_radius * cos(theta - Ogre::Math::PI / sample), height, circle_radius * sin(theta - Ogre::Math::PI / sample));
			object->textureCoord((theta - Ogre::Math::PI/sample)/Ogre::Math::TWO_PI, height);

			object->position(circle_radius * cos(theta - Ogre::Math::PI / sample), -height, circle_radius * sin(theta - Ogre::Math::PI / sample));
			object->normal(circle_radius * cos(theta - Ogre::Math::PI / sample), -height, circle_radius * sin(theta - Ogre::Math::PI / sample));
			object->textureCoord((theta - Ogre::Math::PI/sample)/Ogre::Math::TWO_PI, 0);

			object->position(circle_radius * cos(theta), -height, circle_radius * sin(theta));
			object->normal(circle_radius * cos(theta), -height, circle_radius * sin(theta));
			object->textureCoord(theta/Ogre::Math::TWO_PI, 0);

			object->quad(index, index + 1, index + 2, index + 3);
			object->quad(index + 3, index + 2, index + 1, index);
			index += 4;

			//Build the Bottom Circle of the cylinder
			object->position(bottom_circle_centre);
			object->normal(0, -1, 0);
			object->textureCoord(0, 0);

			object->position(circle_radius * cos(theta), -height, circle_radius * sin(theta));
			object->normal(0, -1, 0);
			object->textureCoord(theta/Ogre::Math::TWO_PI, 0);

			object->position(circle_radius * cos(theta - Ogre::Math::PI / sample), -height, circle_radius * sin(theta - Ogre::Math::PI / sample));
			object->normal(0, -1, 0);
			object->textureCoord((theta - Ogre::Math::PI/sample)/Ogre::Math::TWO_PI, 0);

			object->triangle(index, index + 1, index + 2);
			object->triangle(index + 2, index + 1, index);
			index += 3;
		}
 
		object->end();
		/* Convert triangle list to a mesh */
		object->convertToMesh(object_name);

	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateParticleGeometry(Ogre::String object_name, int num_particles, Ogre::ColourValue color_value, bool isStoringIDAsRed){

	try {
		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create point list for the object */
		object->begin("", Ogre::RenderOperation::OT_POINT_LIST);

		/* Initialize random numbers */
		std::srand(std::time(0));

		/* Create a set of points which will be the particles */
		/* This is similar to drawing a sphere: we will sample points on a sphere, but will allow them to also
		   deviate a bit from the sphere along the normal (change of radius) */
		float trad = 0.4; // Defines the starting point of the particles
        float maxspray = 0.5; // This is how much we allow the points to deviate from the sphere
		float u, v, w, theta, phi, spray; // Work variables
		for (int i = 0; i < num_particles; i++){
			
			// Randomly select three numbers to define a point in spherical coordinates
			u = ((double) rand() / (RAND_MAX));
            v = ((double) rand() / (RAND_MAX));
            w = ((double) rand() / (RAND_MAX));

			// Use u to define the angle theta along one direction of a sphere
            theta = u * 2.0 * 3.1416;
			// Use v to define the angle phi along the other direction of the sphere
			phi = acos(2.0*v - 1.0);
			// Use we to define how much we can deviate from the surface of the sphere (change of radius)
            spray = maxspray*pow((float) w, (float) (1.0/3.0)); // Cubic root of w

			// Define the normal and point based on theta, phi and the spray
            Ogre::Vector3 normal = Ogre::Vector3(spray*cos(theta)*sin(phi), spray*sin(theta)*sin(phi), spray*cos(phi));
			object->position(normal.x*trad, normal.y*trad, normal.z*trad);
			object->normal(normal);

			if (isStoringIDAsRed)
				object->colour(Ogre::ColourValue(((float) i)/((float) num_particles), 0.0, 0.0));
			else
				object->colour(color_value); // We can use the color for debug, if needed
			object->textureCoord(Ogre::Vector2(0.0, 0.0));
		}
		
		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateParticleEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name, Ogre::SceneNode* parent_node){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		if (parent_node == nullptr)
        {
	        parent_node = scene_manager->getRootSceneNode();
        }

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);
		entity->setMaterialName(material_name);
		
		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
		Ogre::SceneNode* scene_node = parent_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

		/* Scale the entity */
		scene_node->scale(0.5, 0.5, 0.5);

		return scene_node;
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

//Create a cone mesh without a bottom circle
void OgreApplication::CreateCone(Ogre::String object_name, Ogre::String material_name) {
	try {
		/* Retrieve scene manager and root scene node */
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	
		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		object = scene_manager->createManualObject(object_name);
		object->setDynamic(false);

		/* Create triangle list for the object */

		float const radius = 0.5,
					sample = 35;
		int const height = 1.0;
 
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);
 
		int index = 0;
		for(float theta = 0; theta <= Ogre::Math::TWO_PI; theta += Ogre::Math::PI / sample) {
			object->position(Ogre::Vector3(0, height, 0));
			object->normal(Ogre::Vector3(0, height, 0));

			object->position(radius * cos(theta), -height, radius * sin(theta));
			object->normal(radius * cos(theta), -height, radius * sin(theta));

			object->position(radius * cos(theta - Ogre::Math::PI / sample), -height, radius * sin(theta - Ogre::Math::PI / sample));
			object->normal(radius * cos(theta - Ogre::Math::PI / sample), -height, radius * sin(theta - Ogre::Math::PI / sample));

			object->triangle(index, index + 1, index + 2);
			object->triangle(index + 2, index + 1, index);
			index += 3;
		}
 
		object->end();
		/* Convert triangle list to a mesh */
		object->convertToMesh(object_name);

	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateFireParticle(Ogre::String object_name, int num_particles){

	try {
		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create point list for the object */
		object->begin("", Ogre::RenderOperation::OT_POINT_LIST);

		/* Initialize random numbers */
		std::srand(std::time(0));

		/* Create a set of points which will be the particles */
		float u, v, w, theta; // Work variables
		for (int i = 0; i < num_particles; i++){
			
			// Randomly select three numbers to define a point on the cone
			u = ((double) rand() / (RAND_MAX));
            v = ((double) rand() / (RAND_MAX));
            w = ((double) rand() / (RAND_MAX));

			// Use u and v to define the point on the cone
			theta = atan(v/u) * Ogre::Math::TWO_PI;
            Ogre::Vector3 normal = Ogre::Vector3(w*cos(theta), -w + 0.7, w*sin(theta));
			object->position(normal);
			object->colour(Ogre::ColourValue(((float) i)/((float) num_particles), 0.0, 1.0 - (((float) i)/((float) num_particles))));
			object->textureCoord(Ogre::Vector2(0.0, 0.0));
			object->normal(normal);
		}

		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateHemisphere(Ogre::String object_name, float radius, int num_samples_theta, int num_samples_phi){

    try {
		/* Create a sphere */

        /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create triangle list for the object */
		object->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Add vertices to the object */
		float theta, phi; // Angles for parametric equation
		Ogre::Vector3 vertex_position;
		Ogre::Vector3 vertex_normal;
		Ogre::ColourValue vertex_color;
		Ogre::Vector2 texture_coord;
		
				
		for (int i = 0; i < num_samples_theta; i++){
			
			theta = Ogre::Math::PI*i/(num_samples_theta-1); // angle theta
			
			for (int j = 0; j < num_samples_phi; j++){
				
				phi = Ogre::Math::PI*j/(num_samples_phi-1); // angle phi
				
				/* Define position, normal and color of vertex */
				vertex_normal = Ogre::Vector3(cos(theta)*sin(phi), sin(theta)*sin(phi), -cos(phi));
				// We need z = -cos(phi) to make sure that the z coordinate runs from -1 to 1 as phi runs from 0 to pi
				// Otherwise, the normal will be inverted
				vertex_position = Ogre::Vector3(vertex_normal.x*radius, 
					                            vertex_normal.y*radius, 
												vertex_normal.z*radius),
				/*vertex_color = Ogre::ColourValue(1.0 - ((float) i / (float) num_samples_theta), 
				                                 (float) i / (float) num_samples_theta, 
				                                 (float) j / (float) num_samples_phi);*/
				vertex_color = Ogre::ColourValue(0.0, 0.0, 1.0);
				texture_coord = Ogre::Vector2(((float)i)/((float)num_samples_theta), 1.0-((float)j)/((float)num_samples_phi));
								
				/* Add them to the object */
				object->position(vertex_position);
				object->normal(vertex_normal);
				object->colour(vertex_color); 
				object->textureCoord(texture_coord);
			}
		}

		/* Add triangles to the object */
		for (int i = 0; i < num_samples_theta; i++){
			for (int j = 0; j < (num_samples_phi-1); j++){
				// Two triangles per quad
				object->triangle(((i + 1) % num_samples_theta)*num_samples_phi + j, 
									i*num_samples_phi + (j + 1),
									i*num_samples_phi + j);
				object->triangle(((i + 1) % num_samples_theta)*num_samples_phi + j, 
									((i + 1) % num_samples_theta)*num_samples_phi + (j + 1), 
									i*num_samples_phi + (j + 1));
			}
		}
		
		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::LoadMaterials(void){

    try {
        
        /* Load materials that can then be assigned to objects in the scene */
        Ogre::String resource_group_name = "MyGame";
        Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
        resource_group_manager.createResourceGroup(resource_group_name);
        bool is_recursive = false;
        resource_group_manager.addResourceLocation(material_directory_g, "FileSystem", resource_group_name, is_recursive);
        resource_group_manager.initialiseResourceGroup(resource_group_name);
        resource_group_manager.loadResourceGroup(resource_group_name);

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}



void OgreApplication::LoadModel(const char *filename, const char *mesh_name){

    try {
        model_loader::LoadModel(filename, mesh_name, ogre_root_.get());
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


Ogre::SceneNode *OgreApplication::CreateEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name, Ogre::SceneNode* parent_node){

    try {
        /* Create one instance or entity for 'object_name' */
        /* The same object can have multiple instances or entities */

        /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        if (parent_node == nullptr)
        {
	        parent_node = scene_manager->getRootSceneNode();
        }

        /* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

        /* Apply a material to the entity */
        entity->setMaterialName(material_name);

        /* Create a scene node for the entity */
        /* The scene node keeps track of the entity's position and orientation */
        Ogre::SceneNode* scene_node = parent_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

        return scene_node;
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::MainLoop(void){

    try {

        /* Main loop to keep the application going */

        ogre_root_->clearEventTimes();

        while(!ogre_window_->isClosed()){
            ogre_window_->update(false);

            ogre_window_->swapBuffers();

            ogre_root_->renderOneFrame();

            Ogre::WindowEventUtilities::messagePump();
        }
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::SetupAnimation(Ogre::String object_name){

    // Set up animation
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

    Ogre::Real duration = Ogre::Math::TWO_PI;
    //Ogre::Real num_steps = 36;
    //Ogre::Real step = duration/num_steps;
    Ogre::Animation* animation = scene_manager->createAnimation("Animation", duration);
    animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);

	// Retrieve scene manager and root scene node
    
	/* Create animation state */
    animation_state_ = scene_manager->createAnimationState("Animation");
    animation_state_->setEnabled(true);
    animation_state_->setLoop(true);

    /* Turn on animating flag */
    animating_ = true;
}


bool OgreApplication::frameRenderingQueued(const Ogre::FrameEvent& fe){
  
    /* This event is called after a frame is queued for rendering */
    /* Do stuff in this event since the GPU is rendering and the CPU is idle */
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
	Ogre::Camera* camera = scene_manager->getCamera("MyCamera");

    /* Keep animating if flag is on */
    if (animating_){
		timer_ += fe.timeSinceLastFrame;
        animation_state_->addTime(fe.timeSinceLastFrame);

		// Helicopter Animation 
		helicopter_->animateHelicopter(fe.timeSinceLastFrame);

		AnimationServices::animateMissileExhaust(timer_, "MissileExhaustMaterial");

		//keep rotating for now
		((TurretModel*)enemies_[NUMBER_OF_ENEMIES-1])->animateTurret();
    }

    /* Capture input */
    keyboard_->capture();
    mouse_->capture();

    /* Handle specific key events */
    if (keyboard_->isKeyDown(OIS::KC_SPACE)){
        space_down_ = true;
    }
    if ((!keyboard_->isKeyDown(OIS::KC_SPACE)) && space_down_){
        animating_ = !animating_;
        space_down_ = false;
    }
    if (keyboard_->isKeyDown(OIS::KC_RETURN)){
        animation_state_->setTimePosition(0);
    }
    if (keyboard_->isKeyDown(OIS::KC_ESCAPE)){
        ogre_root_->shutdown();
        ogre_window_->destroy();
        return false;
    }
	
	if (keyboard_->isKeyDown(OIS::KC_C)) {
		if (isInThirdPerson)
		{
			camera->setPosition(helicopter_->position + helicopter_->GetForward() * 8 - helicopter_->GetUp());
			isInThirdPerson = false;
		}
		else
		{
			camera->setPosition(helicopter_->position + camera_thirdperson_offset);
			isInThirdPerson = true;
		}
	}

	/* Helicopter Controls */
	if (keyboard_->isKeyDown(OIS::KC_UP)) {
		helicopter_->SetCurrentMovement(helicopter_->GetCurrentMovement() + helicopter_->GetForward().normalisedCopy()/30);
	}
	if (keyboard_->isKeyDown(OIS::KC_DOWN)) {
		helicopter_->SetCurrentMovement(helicopter_->GetCurrentMovement() - helicopter_->GetForward().normalisedCopy()/30);
	}
	// TODO: Rotate the helicopter on its forward vector when we move left/right
	if (keyboard_->isKeyDown(OIS::KC_LEFT)) {
		helicopter_->SetCurrentMovement(helicopter_->GetCurrentMovement() - helicopter_->GetRight().normalisedCopy()/30);
	}
	if (keyboard_->isKeyDown(OIS::KC_RIGHT)) {
		helicopter_->SetCurrentMovement(helicopter_->GetCurrentMovement() + helicopter_->GetRight().normalisedCopy()/30);
	}
	if (keyboard_->isKeyDown(OIS::KC_LCONTROL)) {
		helicopter_->SetCurrentMovement(helicopter_->GetCurrentMovement() - helicopter_->GetUp().normalisedCopy()/100);
	}
	if (keyboard_->isKeyDown(OIS::KC_LSHIFT)) {
		helicopter_->SetCurrentMovement(helicopter_->GetCurrentMovement() + helicopter_->GetUp().normalisedCopy()/100);
	}

	if (keyboard_->isKeyDown(OIS::KC_W)) {
		Ogre::Quaternion upRotation = Ogre::Quaternion(Ogre::Degree(-1), helicopter_->GetRight());
		AnimationServices::RotateEntity(helicopter_, upRotation);

		camera->rotate(helicopter_->GetRight(), Ogre::Degree(-1));
		if (isInThirdPerson)
			camera->setPosition(helicopter_->GetThirdPersonCameraPosition());
	}
	if (keyboard_->isKeyDown(OIS::KC_S)) {
		Ogre::Quaternion downRotation = Ogre::Quaternion(Ogre::Degree(1), helicopter_->GetRight());
		AnimationServices::RotateEntity(helicopter_, downRotation);

		camera->rotate(helicopter_->GetRight(), Ogre::Degree(1));
		if (isInThirdPerson)
			camera->setPosition(helicopter_->GetThirdPersonCameraPosition());
	}
	if (keyboard_->isKeyDown(OIS::KC_A)) {
		Ogre::Quaternion leftRotation = Ogre::Quaternion(Ogre::Degree(5), helicopter_->GetUp());
		AnimationServices::RotateEntity(helicopter_, leftRotation);

		camera->rotate(helicopter_->GetUp(), Ogre::Degree(5));
		if (isInThirdPerson)
			camera->setPosition(helicopter_->GetThirdPersonCameraPosition());
		
	}
	if (keyboard_->isKeyDown(OIS::KC_D)) {
		Ogre::Quaternion rightRotation = Ogre::Quaternion(Ogre::Degree(-5), helicopter_->GetUp());
		AnimationServices::RotateEntity(helicopter_, rightRotation);
		camera->rotate(helicopter_->GetUp(), Ogre::Degree(-5));
		if (isInThirdPerson)
			camera->setPosition(helicopter_->GetThirdPersonCameraPosition());
	}

	if (keyboard_->isKeyDown(OIS::KC_Z) && !helicopter_->IsMissileActive()) {
		helicopter_->FireMissile();
	}
	if (keyboard_->isKeyDown(OIS::KC_X) && !helicopter_->IsHomingMissileActive) {
		GameEntity* closestEnemy = getClosestEnemyForHoming();
		//helicopter_->FireMissile();
		scene_manager->destroyManualObject(helicopter_->GetControlPointsName());
		AnimationServices::CreateControlPoints(helicopter_->GetControlPointsName(), helicopter_->GetNumberOfControlPoints(), "SplineParticleMaterial", 
			helicopter_->position, closestEnemy->position, helicopter_->control_points, scene_manager);
		helicopter_->IsHomingMissileActive = true;
		timer_ = 0;
	}

	// Animate the helicopter after changing its variables
	AnimationServices::MoveEntity(helicopter_);

	// Check if any projectiles have come in contact with enemies
	if (helicopter_->IsMissileActive())
	{
		AnimationServices::MoveEntity(helicopter_->GetMissile());
		GameEntity* collidedObject = AnimationServices::DetectMissileCollision(helicopter_->GetMissile(), enemies_);
		if (collidedObject != nullptr)
		{
			collidedObject->ToggleVisibility(false);
		}
		// If the missile is too far away, deactivate it
		if (helicopter_->GetMissile()->position.distance(helicopter_->position) > 100)
			helicopter_->DeactivateMissile();
	}

	if (helicopter_->IsHomingMissileActive)
	{
		Ogre::MaterialPtr mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("SplineParticleMaterial"));
		mat->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setNamedConstant("timer", timer_);

		if (timer_ > 2)
		{
			timer_ = 0;
			helicopter_->IsHomingMissileActive = false;
		}
	}

	for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
	{
		if (enemies_[i]->isMovable) {
			if (enemies_[i]->position.distance(helicopter_->position) < 150)
			{
				enemies_[i]->SetCurrentMovement((helicopter_->position - enemies_[i]->position).normalisedCopy());
			}
			else
			{
				if (enemies_[i]->time_since_direction_change == 0)
				{
					enemies_[i]->SetCurrentMovement(enemies_[i]->GetCurrentMovement() + (rand() % 1 - 1) * enemies_[i]->GetForward().normalisedCopy()/30);
					Ogre::Quaternion rotation = Ogre::Quaternion(Ogre::Degree(rand() % 90 - 90), enemies_[i]->GetUp());
					AnimationServices::RotateEntity(enemies_[i], rotation);
				}
				enemies_[i]->time_since_direction_change++;
				if (enemies_[i]->time_since_direction_change > 10)
					enemies_[i]->time_since_direction_change = 0;
			}
			AnimationServices::MoveEntity(enemies_[i]);
		}
	}

	// Displace the camera along with the helicopter
	camera->move(helicopter_->GetCurrentMovement());

    return true;
}

GameEntity* OgreApplication::getClosestEnemyForHoming()
{
	Ogre::Vector3 homingPoint = helicopter_->position + helicopter_->GetForward() * 15;

	GameEntity* closestEnemy = enemies_[0];
	float closestDistance = 99999;

	for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
	{
		float distanceToPoint = homingPoint.distance(enemies_[i]->GetParts()[0]->getPosition());
		if (distanceToPoint < closestDistance)
		{
			closestDistance = distanceToPoint;
			closestEnemy = enemies_[i];
		}
	}

	return closestEnemy;
}


void OgreApplication::windowResized(Ogre::RenderWindow* rw){

    /* Update the window and aspect ratio when the window is resized */
    int width = rw->getWidth(); 
    int height = rw->getHeight();
      
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::Camera* camera = scene_manager->getCamera("MyCamera");

    if (camera != NULL){
        //std::cout << "1 " << (double)width/height << std::endl;
        camera->setAspectRatio((double)width/height);
    }

    const OIS::MouseState &ms = mouse_->getMouseState();
    ms.width = width;
    ms.height = height;

    ogre_window_->resize(width, height);
    ogre_window_->windowMovedOrResized();
    ogre_window_->update();
}

//Create the meshes that we need
void OgreApplication::InitMesh(void) {
	CreateCylinder("Cylinder", 1, "ShinyCylinderTextureMaterial");
	CreateCube("Cube", "ShinyTextureMaterial");
	CreateCone("Cone", "MissileMaterial");
	CreateFireParticle("missile_exhaust", 20000);
	CreateHemisphere("Hemisphere");
	CreateParticleGeometry("SmokeParticles", 20000, Ogre::ColourValue(150, 0, 0), true);
}

/*
 *	Method to load all models
 */
void OgreApplication::InitializeAssets(void)
{
	InitMesh();

	#pragma region helicopter
	helicopter_ = new HelicopterModel();
	Ogre::SceneNode** newHelicopter = new Ogre::SceneNode*[7];

    // Create the parts
    newHelicopter[0] = CreateEntity("CylinderInstance1", "Cylinder", "ShinyCylinderTextureMaterial");
	newHelicopter[1] = CreateEntity("MainRotorblade", "Cylinder", "ShinyCylinderTextureMaterial", newHelicopter[0]);
	newHelicopter[2] = CreateEntity("BodyTop", "Cube", "ShinyTextureMaterial", newHelicopter[0]);
	newHelicopter[3] = CreateEntity("BodyBottom", "Cube", "ShinyTextureMaterial", newHelicopter[0]);
	newHelicopter[4] = CreateEntity("TailBody", "Cylinder", "ShinyCylinderTextureMaterial", newHelicopter[0]);
	newHelicopter[5] = CreateEntity("TailRotorblade", "Cylinder", "ShinyCylinderTextureMaterial", newHelicopter[0]);

	//newHelicopter[6] = newHelicopter[0]->createChildSceneNode(camera_thirdperson_offset);
	newHelicopter[6] = newHelicopter[0]->createChildSceneNode("CameraPlaceholder", camera_thirdperson_offset);

	helicopter_->SetParts(newHelicopter);
	helicopter_->initializeHelicopter_OgreSceneGraph();
	#pragma endregion

	#pragma region missile
	MissileModel* heliMissile = new MissileModel();
	Ogre::SceneNode** missileModel = new Ogre::SceneNode*[7];

	//Create the parts for the missile
	missileModel[0] = CreateEntity("missile_body", "Cylinder", "MissileMaterial");
	missileModel[1] = CreateEntity("missile_top", "Cone", "MissileMaterial", missileModel[0]);
	missileModel[2] = CreateEntity("missile_wing_1", "Cube", "MissileMaterial", missileModel[0]);
	missileModel[3] = CreateEntity("missile_wing_2", "Cube", "MissileMaterial", missileModel[0]);
	missileModel[4] = CreateEntity("missile_wing_3", "Cube", "MissileMaterial", missileModel[0]);
	missileModel[5] = CreateEntity("missile_wing_4", "Cube", "MissileMaterial", missileModel[0]);
	missileModel[6] = CreateParticleEntity("missile_exhaust", "missile_exhaust", "MissileExhaustMaterial", missileModel[0]);

	missileModel[0]->setVisible(false);
	heliMissile->SetParts(missileModel);
	//build the missile model
	heliMissile->buildMissileModel();
	helicopter_->SetMissile(heliMissile);
	#pragma endregion

	#pragma region turret
	TurretModel* turret = new TurretModel();
	Ogre::SceneNode** turretModel = new Ogre::SceneNode*[2];

	turretModel[0] = CreateEntity("turret_body", "Hemisphere", "TurretMaterial");
	turretModel[1] = CreateEntity("turret_cannon", "Cylinder", "TurretMaterial", turretModel[0]);
	turret->SetParts(turretModel);
	turret->buildTurretModel(Ogre::Vector3(-120, 90, -30));
	//Put the turret to the end of the array for now 
	enemies_[NUMBER_OF_ENEMIES-1] = turret;
	enemies_[NUMBER_OF_ENEMIES-1]->isMovable = false;
	#pragma endregion


	#pragma region tank
	TankModel* tank = new TankModel();
	Ogre::SceneNode** tankModel = new Ogre::SceneNode*[3];

	tankModel[0] = CreateEntity("tank_body", "Cube", "TurretMaterial");
	tankModel[1] = CreateEntity("tank_top", "Cylinder", "TurretMaterial", tankModel[0]);
	tankModel[2] = CreateEntity("tank_cannon", "Cylinder", "TurretMaterial", tankModel[1]);

	tank->SetParts(tankModel);
	tank->buildTankModel(Ogre::Vector3(-120, 90, -30));
	#pragma endregion

	#pragma region enemies
	for (int i = 0; i < NUMBER_OF_ENEMIES-1; i++)
	{
		Ogre::String entity_name, prefix("Enemy_");;
		entity_name = prefix + Ogre::StringConverter::toString(i);
		Ogre::SceneNode** enemyParts = new Ogre::SceneNode*[1];

		Ogre::Vector3 enemy_position = Ogre::Vector3(-300.0 + rand() / 300, -10 + rand() / 10, -300.0 + rand() / 300);

		// TODO: For now i'm just using cubes for the meshes, load actual tanks and whatnot for later.
		enemyParts[0] = CreateEntity(entity_name, "Cube", "ShinyTextureMaterial");
		enemyParts[0]->setPosition(enemy_position);
		enemyParts[0]->scale(10, 10, 10);
		enemies_[i] = new GameEntity();
		enemies_[i]->position = enemy_position;
		enemies_[i]->SetRadius(7);
		enemies_[i]->SetNumOfParts(1);
		enemies_[i]->SetParts(enemyParts);
	}
	#pragma endregion

	CreateParticleEntity(helicopter_->GetSplineParticleName(), "SmokeParticles", "SplineParticleMaterial");
}

void OgreApplication::LoadTerrain(void) {
	LoadModel("desert.obj", "desert");
	LoadModel("building.obj", "building");
	LoadModel("top.obj", "top");
	LoadModel("helipad.obj", "helipad");
	LoadModel("sky.obj", "sky");

	Ogre::SceneNode *sky = CreateEntity("sky", "sky", "SkyMaterial");
	floor_ = CreateEntity("terrain", "desert", "TerrainMaterial", sky);
	Ogre::SceneNode *building = CreateEntity("building", "building", "BuildingMaterial", floor_);
	Ogre::SceneNode *building_top = CreateEntity("top", "top", "TopMaterial", building);
	Ogre::SceneNode *helipad = CreateEntity("helipad", "helipad", "HelipadMaterial", building);

	sky->setScale(111, 111, 111);
	sky->translate(10, -20, -5);
	sky->pitch(Ogre::Angle(180));

	floor_->setInheritOrientation(false);

	/*
	LoadModel("desert.obj", "desert");
	floor_ = CreateEntity("terrain", "desert", "TerrainMaterial");
	floor_->setScale(100, 100, 100);
	floor_->setPosition(10, -20, -5);
	*/

	building->setInheritScale(false);
	building->setScale(50, 50, 50);
	building->translate(0, 0.15, 0);
}

} // namespace ogre_application;