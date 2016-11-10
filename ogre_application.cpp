#include "ogre_application.h"
#include "bin/path_config.h"
#include "build/AnimationServices.h"

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
float camera_far_clip_distance_g = 500.0;
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

	camera_thirdperson_offset = Ogre::Vector3(-30, 20.5, 0);
	isInThirdPerson = true;

	NUMBER_OF_ENEMIES = 30;
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

		#pragma region Vertices and mesh positions
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
		Ogre::ColourValue clr1(0.0, 0.0, 0.9);
		Ogre::ColourValue clr2(0.0, 0.0, 0.8);
		
		/* This construction only partially uses shared vertices, so that we can assign appropriate vertex normals
		   to each face */
		/* Each face of the cube is defined by four vertices (with the same normal) and two triangles */
		object->position(v0);
		object->normal(n0);
		object->textureCoord(0, 0);
		object->colour(clr0);

		object->position(v1);
		object->normal(n0);
		object->textureCoord(1, 1);
		object->colour(clr0);

		object->position(v2);
		object->normal(n0);
		object->textureCoord(1, 1);
		object->colour(clr0);

		object->position(v3);
		object->normal(n0);
		object->textureCoord(0, 1);
		object->colour(clr0);
		
		object->position(v1);
		object->normal(n1);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v5);
		object->normal(n1);
		object->textureCoord(1, 0);
		object->colour(clr1);

		object->position(v6);
		object->normal(n1);
		object->textureCoord(1, 1);
		object->colour(clr1);

		object->position(v2);
		object->normal(n1);
		object->textureCoord(0, 1);
		object->colour(clr1);

		object->position(v5);
		object->normal(n2);
		object->textureCoord(0, 0);
		object->colour(clr2);

		object->position(v4);
		object->normal(n2);
		object->textureCoord(1, 0);
		object->colour(clr2);
		
		object->position(v7);
		object->normal(n2);
		object->textureCoord(1, 1);
		object->colour(clr2);

		object->position(v6);
		object->normal(n2);
		object->textureCoord(0, 1);
		object->colour(clr2);

		object->position(v4);
		object->normal(n3);
		object->textureCoord(0, 0);
		object->colour(clr0);

		object->position(v0);
		object->normal(n3);
		object->textureCoord(1, 0);
		object->colour(clr0);

		object->position(v3);
		object->normal(n3);
		object->textureCoord(1, 1);
		object->colour(clr0);

		object->position(v7);
		object->normal(n3);
		object->textureCoord(0, 1);
		object->colour(clr0);

		object->position(v3);
		object->normal(n4);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v2);
		object->normal(n4);
		object->textureCoord(1, 0);
		object->colour(clr1);

		object->position(v6);
		object->normal(n4);
		object->textureCoord(1, 1);
		object->colour(clr1);

		object->position(v7);
		object->normal(n4);
		object->textureCoord(0, 1);
		object->colour(clr1);

		object->position(v1);
		object->normal(n5);
		object->textureCoord(0, 0);
		object->colour(clr2);

		object->position(v0);
		object->normal(n5);
		object->textureCoord(1, 0);
		object->colour(clr2);

		object->position(v4);
		object->normal(n5);
		object->textureCoord(1, 1);
		object->colour(clr2);

		object->position(v5);
		object->normal(n5);
		object->textureCoord(0, 1);
		object->colour(clr2);

		#pragma endregion

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

void OgreApplication::CreateCylinder(Ogre::String object_name, float circle_radius, Ogre::String material_name)
{
	try
	{
		const int num_loop_samples = 20;
		/* Retrieve scene manager and root scene node */
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        
		Ogre::Degree theta = (Ogre::Degree)0;
		Ogre::Degree alpha = (Ogre::Degree)360/num_loop_samples;
		float cylinder_height = 1;

		Ogre::Vector3 bottom_circle_center, top_circle_center;
		Ogre::Vector3 bottom_circle[num_loop_samples], top_circle[num_loop_samples];

		Ogre::Vector2 texture_coord;

		bottom_circle_center.x = 0;
		bottom_circle_center.y = -cylinder_height/2;
		bottom_circle_center.z = 0;

		top_circle_center.x = 0;
		top_circle_center.y = cylinder_height/2;
		top_circle_center.z = 0;

		for (int i = 0 ; i < num_loop_samples; i++)
		{
			theta = theta + alpha;
			bottom_circle[i].x = Ogre::Math::Sin(theta) * circle_radius;
			bottom_circle[i].y = bottom_circle_center.y;
			bottom_circle[i].z = Ogre::Math::Cos(theta) * circle_radius;

			top_circle[i].x = Ogre::Math::Sin(theta) * circle_radius;
			top_circle[i].y = top_circle_center.y;
			top_circle[i].z = Ogre::Math::Cos(theta) * circle_radius;
		}

        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

		/* top circle */
        object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_FAN);
		object->colour(Ogre::ColourValue(0.0f, 0.0f, 1.0f));
		
		object->position(bottom_circle_center);

		for (int i = 0; i < num_loop_samples; i++)
		{
			object->position(top_circle[i]);
			object->normal(top_circle_center);
		}
		object->position(top_circle[0]);
		object->end();

		/* Curved part of the cylinder */
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);
		object->colour(Ogre::ColourValue(0.0f, 0.0f, 0.8f));

		int loop_count;
		for (loop_count = 0; loop_count < num_loop_samples-1; loop_count++)
		{
			Ogre::Vector3 norm1 = top_circle[loop_count];
			norm1.y = 0;
			Ogre::Vector3 norm2 = top_circle[loop_count + 1];
			norm2.y = 0;

			object->position(top_circle[loop_count]);
			object->normal(norm1);
			object->position(bottom_circle[loop_count]);
			object->normal(norm1);
			object->position(bottom_circle[loop_count+1]);
			object->normal(norm2);

			object->position(top_circle[loop_count]);
			object->normal(norm1);
			object->position(bottom_circle[loop_count+1]);
			object->normal(norm2);
			object->position(top_circle[loop_count+1]);
			object->normal(norm2);

			for (int j = 0; j < cylinder_height; j++)
				texture_coord = Ogre::Vector2(((float)loop_count)/((float)num_loop_samples), 1.0-((float)j)/((float)cylinder_height));

			object->textureCoord(texture_coord);
			
		}

		object->position(top_circle[loop_count]);
		object->position(bottom_circle[loop_count]);
		object->position(bottom_circle[0]);

		object->position(top_circle[loop_count]);
		object->position(bottom_circle[0]);
		object->position(top_circle[0]);

		object -> end();

		/* Bottom circle */
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_FAN);
		object->colour(Ogre::ColourValue(0.0f, 0.0f, 1.0f));
		
		object->position(top_circle_center);

		for (int i = num_loop_samples-1; i > 0; i--)
		{
			object->position(bottom_circle[i]);
			object->normal(bottom_circle_center);
		}
		object->position(bottom_circle[num_loop_samples-1]);
		object->end();

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
        animation_state_->addTime(fe.timeSinceLastFrame);

		// Helicopter Animation 
		helicopter_->animateHelicopter(fe.timeSinceLastFrame);
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

	Ogre::Vector3 oldCameraPos = camera->getPosition();
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

	// Animate the helicopter after changing its variables
	AnimationServices::MoveEntity(helicopter_);
	if (helicopter_->IsMissileActive())
	{
		AnimationServices::MoveEntity(helicopter_->GetMissile());
		GameEntity* collidedObject = AnimationServices::DetectMissileCollision(helicopter_->GetMissile(), enemies_);
		if (collidedObject != nullptr)
		{
			collidedObject->ToggleVisibility(false);
		}
		if (helicopter_->GetMissile()->position.distance(helicopter_->position) > 100)
			helicopter_->DeactivateMissile();
	}

	camera->move(helicopter_->GetCurrentMovement());

    return true;
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

void OgreApplication::InitializeAssets(void)
{
	#pragma region helicopter
	helicopter_ = new HelicopterModel();
	Ogre::SceneNode** newHelicopter = new Ogre::SceneNode*[7];

	CreateCylinder("Cylinder", 1, "ShinyCylinderTextureMaterial");
	CreateCube("Cube", "ShinyTextureMaterial");
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

	MissileModel* heliMissile = new MissileModel();
	Ogre::SceneNode** missileModel = new Ogre::SceneNode*[1];

	//LoadModel("missile.obj", "missile");
	missileModel[0] = CreateEntity("helicopter_missile", "Cube", "MissileMaterial");
	missileModel[0]->setVisible(false);
	heliMissile->SetParts(missileModel);
	helicopter_->SetMissile(heliMissile);

	#pragma region enemies
	for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
	{
		Ogre::String entity_name, prefix("Enemy_");;
		entity_name = prefix + Ogre::StringConverter::toString(i);
		Ogre::SceneNode** enemyParts = new Ogre::SceneNode*[1];

		Ogre::Vector3 enemy_position = Ogre::Vector3(-300.0 + rand() / 300, -10 + rand() / 10, -300.0 + rand() / 300);

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
}

void OgreApplication::LoadTerrain(void) {
	LoadModel("desert.obj", "desert");
	LoadModel("building.obj", "building");
	LoadModel("top.obj", "top");
	LoadModel("helipad.obj", "helipad");

	floor_ = CreateEntity("terrain", "desert", "TerrainMaterial");
	Ogre::SceneNode *building = CreateEntity("building", "building", "BuildingMaterial", floor_);
	Ogre::SceneNode *building_top = CreateEntity("top", "top", "TopMaterial", building);
	Ogre::SceneNode *helipad = CreateEntity("helipad", "helipad", "HelipadMaterial", building);

	floor_->setScale(111, 111, 111);
	floor_->setPosition(10, -20, -5);

	/*
	LoadModel("desert.obj", "desert");
	floor_ = CreateEntity("terrain", "desert", "TerrainMaterial");
	floor_->setScale(100, 100, 100);
	floor_->setPosition(10, -20, -5);
	*/

	building->setInheritScale(false);
	building->setScale(6, 6, 6);
	building->translate(0, 0.15, 0);
}
} // namespace ogre_application;