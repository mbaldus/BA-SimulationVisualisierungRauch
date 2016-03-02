#ifndef __CVK_CAMERA_H
#define __CVK_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>
#include <GLFW\glfw3.h>

class Camera
	{
	public:
		Camera();
		~Camera();

		void update( GLFWwindow* window);

		glm::mat4 *getView();
		void getView( glm::vec3 *x, glm::vec3 *y, glm::vec3 *z, glm::vec3 *pos);
		void setView( glm::mat4 *view);
		void setWidthHeight( int width, int height);
		void getWidthHeight( int *width, int *height);

		void lookAt( glm::vec3 position, glm::vec3 center, glm::vec3 up);

	protected:
		int m_width, m_height;
		glm::mat4 m_viewmatrix;
};

#endif /* __CVK_CAMERA_H */
