#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include <string>
#include <OgrePrerequisites.h>

namespace COMP3501_project
{
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

	class GameController
	{
	public:
		GameController();
		void MainGameLoop();

	};
}

#endif