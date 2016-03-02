#include "GLTools.h"

GLFWwindow* GLTools::generateWindow(int width, int height, int posX, int posY, std::string name) {
	
	glfwInit();

	#ifdef CVARK_USE_CORE_PROFILE_330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glewExperimental = GL_TRUE;
    #endif

	GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	glfwSetWindowPos(window, posX, posY);
	glfwSetWindowSize(window, width, height);
	glfwMakeContextCurrent(window);

	glewInit();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	registerDefaultGLFWCallbacks(window);

	return window;
}

bool GLTools::shouldClose(GLFWwindow* window)
{

	if (glfwWindowShouldClose(window) || (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GLTools::swapBuffers(GLFWwindow* window)
{
	glfwSwapBuffers(window);
    glfwPollEvents();
}

void GLTools::destroyWindow(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLTools::render(GLFWwindow* window, std::function<void (double)> loop) {
	float lastTime = 0.0;
	while ( !shouldClose(window) ) {
		float currentTime =static_cast<float>(glfwGetTime());
		loop(currentTime - lastTime);
		lastTime = currentTime;

		swapBuffers(window);
	}
}

void GLTools::setKeyCallback(GLFWwindow* window, std::function<void (int, int, int, int)> func) {
	static std::function<void (int, int, int, int)> func_bounce = func;
	glfwSetKeyCallback(window, [] (GLFWwindow* w, int k, int s, int a, int m) {
		func_bounce(k, s, a, m);
	});
}

void GLTools::setMouseButtonCallback(GLFWwindow* window, std::function<void (int, int, int)> func) {
	static std::function<void (int, int, int)> func_bounce = func;
	glfwSetMouseButtonCallback(window, [] (GLFWwindow* w, int b, int a, int m) {
		func_bounce(b, a, m);
	});
}

void GLTools::setCharCallback(GLFWwindow* window, std::function<void (unsigned int)> func) {
	static std::function<void (unsigned int)> func_bounce = func;
	glfwSetCharCallback(window, [] (GLFWwindow* w, unsigned int c) {
		func_bounce(c);
	});
}

void GLTools::setCursorPosCallback(GLFWwindow* window, std::function<void (double, double)> func) {
	static std::function<void (double, double)> func_bounce = func;
	glfwSetCursorPosCallback(window, [] (GLFWwindow* w, double x, double y) {
		func_bounce(x, y);
	});
}

void GLTools::setScrollCallback(GLFWwindow* window, std::function<void (double, double)> func) {
	static std::function<void (double, double)> func_bounce = func;
	glfwSetScrollCallback(window, [] (GLFWwindow* w, double x, double y) {
		func_bounce(x, y);
	});
}

void GLTools::setCursorEnterCallback(GLFWwindow* window, std::function<void (int)> func) {
	static std::function<void (int)> func_bounce = func;
	glfwSetCursorEnterCallback(window, [] (GLFWwindow* w, int e) {
		func_bounce(e);
	});
}

int GLTools::getWidth(GLFWwindow* window) {
	int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    return w;
}

int GLTools::getHeight(GLFWwindow* window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    return h;
}

glm::vec2 GLTools::getResolution(GLFWwindow* window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    return glm::vec2(float(w), float(h));
}

float GLTools::getRatio(GLFWwindow* window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    return float(w)/float(h);
}

GLenum GLTools::checkGLError(bool printIfNoError)
{
	GLenum error = glGetError();
	
	switch (error) 
	{
	case GL_INVALID_VALUE:
		std::cout <<"GL_INVALID_VALUE , GLTools::checkGLError()" << std::endl;
		break;
	case GL_INVALID_ENUM:
		std::cout <<"GL_INVALID_ENUM , GLTools::checkGLError()" << std::endl;
		break;
	case GL_INVALID_OPERATION:
		std::cout <<"GL_INVALID_OPERATION , GLTools::checkGLError()" << std::endl;
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout <<"GL_INVALID_FRAMEBUFFER_OPERATION , GLTools::checkGLError()" << std::endl;
		break;
	case GL_OUT_OF_MEMORY:
		std::cout <<"GL_OUT_OF_MEMORY , GLTools::checkGLError()" << std::endl;
		break;		
	case GL_NO_ERROR:				
		if (printIfNoError)
		{
			std::cout <<"GL_NO_ERROR , GLTools::checkGLError()" << std::endl; // do not use error stream for this
		}
		break;
	}
	return error;
}


std::string GLTools::decodeGLError(GLenum error)
{
	switch (error) 
	{
	case GL_INVALID_VALUE:
		return std::string("GL_INVALID_VALUE");
	case GL_INVALID_ENUM:
		return std::string("GL_INVALID_ENUM");
	case GL_INVALID_OPERATION:
		return std::string("GL_INVALID_OPERATION");
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return std::string("GL_INVALID_FRAMEBUFFER_OPERATION");
	case GL_OUT_OF_MEMORY:
		return std::string("GL_OUT_OF_MEMORY");
	case GL_NO_ERROR:
		return std::string("GL_NO_ERROR");
	}

	return std::string(); // not a valid error state
}

/****************** DEFAULT CALLBACK HANDLING ***************/

namespace {

	// create a function vector to register all callable functions (this is probably not gonna work as expected)
	std::vector< std::function<void (int, int, int, int) > > keyCallbacks;
	std::vector< std::function<void (int, int, int) > > 	 mouseButtonCallbacks;
	std::vector< std::function<void (double, double) > > 	 cursorPosCallbacks;
	std::vector< std::function<void (double, double) > >     scrollCallbacks;

	// create a callback method to be bound to glfw
	void keyCallback(int key, int scan, int action, int mods)
	{
		for (auto func : keyCallbacks)
		{
			func(key, scan, action, mods);
		}
	}

	// create a callback method to be bound to glfw
	void mouseButtonCallback(int button, int action, int mods)
	{
		for (auto func : mouseButtonCallbacks)
		{
			func(button, action, mods);
		}
	}

	// create a callback method to be bound to glfw
	void cursorPosCallback(double x, double y)
	{
		for (auto func : cursorPosCallbacks)
		{
			func(x, y);
		}
	}

	// create a callback method to be bound to glfw
	void scrollCallback(double x, double y)
	{
		for (auto func : scrollCallbacks)
		{
			func(x, y);
		}
	}

} //anonymous namespace to hide callback functions to outside of this file


void GLTools::registerDefaultGLFWCallbacks(GLFWwindow* window)
{
	setKeyCallback(window, &keyCallback);
	setMouseButtonCallback(window, &mouseButtonCallback);
	setCursorPosCallback(window, &cursorPosCallback);
	setScrollCallback(window, &scrollCallback);
}


void GLTools::addKeyCallbackFunction( std::function<void(int, int, int, int)> cbFunction )
{
	keyCallbacks.push_back(cbFunction);
}

void GLTools::addMouseButtonCallbackFunction( std::function< void(int, int, int)> cbFunction )
{
	mouseButtonCallbacks.push_back(cbFunction);
}


void GLTools::addCursorPosCallbackFunction( std::function<void(double, double)> cbFunction)
{
	cursorPosCallbacks.push_back(cbFunction);
}


void GLTools::addScrollCallbackFunction( std::function<void(double, double)> cbFunction )
{
	scrollCallbacks.push_back(cbFunction);
}