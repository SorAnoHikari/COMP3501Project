#ifndef OGRE_APPLICATION_H_
#define OGRE_APPLICATION_H_

#include <exception>
#include <string>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include "model_loader.h"
#include "build/GameEntity.h"
#include "build/HelicopterModel.h"

namespace COMP3501_project {


    /* A useful type to define */
    typedef std::vector<Ogre::String> Strings;

    /* Our exception type */
    class OgreAppException: public std::exception
    {
        private:
            std::string message_;
        public:
            OgreAppException(std::string message) : message_(message) {};
                        ~OgreAppException() throw () {};
            virtual const char* what() const throw() { return message_.c_str(); };
    };

    /* Our Ogre application */
    class OgreApplication :
        public Ogre::FrameListener, // Derive from FrameListener to be able to have render event callbacks
        public Ogre::WindowEventListener // Derive from FrameListener to be able to have window event callbacks
        {

        public:
            OgreApplication(void);
            // Call Init() before running the main loop
            void Init(void); 
            // Load a mesh from an obj file and add it to the available resources
            void LoadModel(const char *filename, const char *mesh_name);
            // Create an entity of an object that we can show on the screen
            Ogre::SceneNode *CreateEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name);
            // Setup animation for an object
            void SetupAnimation(Ogre::String object_name);
            // Keep application active
            void MainLoop(void); 

			void OgreApplication::CreateCylinder(Ogre::String object_name, float circle_radius, Ogre::String material_name);
			void OgreApplication::CreateCube(Ogre::String object_name, Ogre::String material_name);
			void OgreApplication::CreateSquare(Ogre::String object_name, Ogre::String material_name);

			void OgreApplication::InitializeAssets(void);
			void OgreApplication::createScene();

        private:
            // Create root that allows us to access Ogre commands
            std::auto_ptr<Ogre::Root> ogre_root_;
            // Main Ogre window
            Ogre::RenderWindow* ogre_window_;

			Ogre::SceneNode* floor_;
			HelicopterModel* helicopter_;
			// When we move it forward, it's just going to move in the x-z plane, so we only need a 2d vec
			Ogre::Vector3 helicopter_orientation_;

            // For animating an object
            Ogre::AnimationState *animation_state_; // Keep state of the animation
            bool animating_; // Whether animation is on or off
            bool space_down_; // Whether space key was pressed

			enum Direction last_dir_;

            // Input managers
            OIS::InputManager *input_manager_;
            OIS::Mouse *mouse_;
            OIS::Keyboard *keyboard_;

			// Terrain related things
			
			void OgreApplication::createFrameListener();
			void OgreApplication::destroyScene();

			void OgreApplication::defineTerrain(long x, long y);
			void OgreApplication::initBlendMaps(Ogre::Terrain* terrain);
			void OgreApplication::configureTerrainDefaults(Ogre::Light* light);
			void OgreApplication::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
			bool mTerrainsImported;
			Ogre::TerrainGroup* mTerrainGroup;
			Ogre::TerrainGlobalOptions* mTerrainGlobals;

            /* Methods to initialize the application */
            void InitRootNode(void);
            void InitPlugins(void);
            void InitRenderSystem(void);
            void InitWindow(void);
            void InitViewport(void);
            void InitEvents(void);
            void InitOIS(void);
            void LoadMaterials(void);

            /* Methods to handle events */
            bool frameRenderingQueued(const Ogre::FrameEvent& fe);
            void windowResized(Ogre::RenderWindow* rw);

    }; // class OgreApplication

} // namespace ogre_application;

#endif // OGRE_APPLICATION_H_
