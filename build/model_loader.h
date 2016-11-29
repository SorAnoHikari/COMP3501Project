#ifndef MODEL_LOADER_H_
#define MODEL_LOADER_H_

#include <exception>
#include <string>
#include <fstream>
#include <sstream>

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"
#include "OGRE/OgreHardwarePixelBuffer.h"

namespace model_loader {

/* A mesh stored in memory */
struct Face {
    int i[3];
    int n[3];
    int t[3];
};

struct Quad {
    int i[4];
    int n[4];
    int t[4];
};

struct Mesh {
    std::vector<Ogre::Vector3> position;
    std::vector<Ogre::Vector3> normal;
    std::vector<Ogre::Vector2> tex_coord;
    std::vector<Face> face;
};

/* Loads a mesh in obj format, and adds it to the list of OGRE resources
 * with the provided 'mesh_name' */
void LoadModel(const char *filename, const char *mesh_name, Ogre::Root *root);

/* Helper functions */
/* Trim any character in to_trim from the beginning and end of str */
void string_trim(std::string str, std::string to_trim);
/* Split string into substrings according to characters in separator */
std::vector<std::string> string_split(std::string str, std::string separator);
/* Split string into substrings according to characters in separator. A
 * split is performed when one separator character is found, rather than
 * a sequence of separators */
std::vector<std::string> string_split_once(std::string str, std::string separator);
/* Print a mesh stored internally */
void print_mesh(Mesh &mesh);
/* Conversion between strings and numbers */
template <typename T> std::string num_to_str(T num);
template <typename T> T str_to_num(const std::string &str);

} // namespace model_loader;

#endif // MODEL_LOADER_H_
