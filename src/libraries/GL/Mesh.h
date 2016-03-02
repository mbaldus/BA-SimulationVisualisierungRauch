#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Mesh
{
public:
   Mesh();

   virtual void draw();

   void bind();
   void unbind();

private:
	GLuint createVBO(std::vector<float> content, GLuint dimensions, GLuint vertexAttributepointer);

protected:
	GLuint m_vertexbuffer;
	GLuint m_normalbuffer;
	GLuint m_uvbuffer;

	std::vector<glm::vec4> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_uvs;

	GLenum m_mode;
	GLuint m_vao;

};


#endif //MESH_H

    