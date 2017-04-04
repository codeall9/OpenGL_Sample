#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <GL/glut.h>

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

GLuint setup_texture(unsigned char * data, int width, int height);

#endif