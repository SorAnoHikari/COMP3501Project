#ifndef OGRE_APPLICATION_H_
#define OGRE_APPLICATION_H_

#include <exception>
#include <string>

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include "model_loader.h"

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

        private:
            // Create root that allows us to access Ogre commands
            std::auto_ptr<Ogre::Root> ogre_root_;
            // Main Ogre window
            Ogre::RenderWindow* ogre_window_;

            // For animating an object
            Ogre::AnimationState *animation_state_; // Keep state of the animation
            bool animating_; // Whether animation is on or off
            bool space_down_; // Whether space key was pressed

            // Input managers
            OIS::InputManager *input_manager_;
            OIS::Mouse *mouse_;
            OIS::Keyboard *keyboard_;

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
