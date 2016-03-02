#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"

class Texture
{
public:
   Texture(std::string path);
   ~Texture();

   GLuint getHandle();

   GLuint load(std::string path);
   void setTexture(int width, int height, int bytesPerPixel, unsigned char *data);

protected:
	GLuint m_textureHandle;
	int m_width;
	int m_height;
	int m_bytesPerPixel;
	unsigned char *m_data;
};


#endif //TEXTURE_H
