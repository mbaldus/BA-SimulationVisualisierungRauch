#ifndef BUFFER_H
#define BUFFER_H
#include <GL/glew.h>

//create a VBO
GLuint createVBO(const void* data, int dataSize, GLuint dimensions, GLuint vertexAttributePointer);

#endif  //BUFFER_H