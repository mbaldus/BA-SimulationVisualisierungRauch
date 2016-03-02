#include "buffer.h"

GLuint createVBO(const void* data, int dataSize, GLuint dimensions, GLuint vertexAttributePointer)
{
	GLuint vbo = 0; //0 is reserved

	glGenBuffers(1, &vbo);	//create VBO
	glBindBuffer(GL_ARRAY_BUFFER,vbo); //activate vbo id to use
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW); //upload data to video card
	
	glVertexAttribPointer(vertexAttributePointer, dimensions, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexAttributePointer);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	return vbo; //return VBO id
}



