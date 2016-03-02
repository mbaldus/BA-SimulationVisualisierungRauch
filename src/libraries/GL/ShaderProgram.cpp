#include "ShaderProgram.h"
   
ShaderProgram::ShaderProgram(std::string vertexshader,std::string fragmentshader)
{
	// Initially, we have zero shaders attached to the program
	m_shadercount = 0;

	m_shaderProgramHandle = glCreateProgram();

	//set up shaders
	Shader vertexShader(GL_VERTEX_SHADER);
	vertexShader.loadFromFile(SHADERS_PATH + vertexshader);
	vertexShader.compile();

	Shader fragmentShader(GL_FRAGMENT_SHADER);
	fragmentShader.loadFromFile(SHADERS_PATH + fragmentshader);
	fragmentShader.compile();

	//set up shaderprogram
	attachShader(vertexShader);
	attachShader(fragmentShader);
	link();
}

ShaderProgram::~ShaderProgram()
{

}
    
void ShaderProgram::attachShader(Shader shader)
{	
	// Increment the number of shaders we have associated with the program
	m_shadercount++;
	// Attach the shader to the program
	// Note: We identify the shader by its unique Id value
	glAttachShader( m_shaderProgramHandle, shader.getId());

}

void ShaderProgram::link()
{
	// If we have at least two shaders (like a vertex shader and a fragment shader)...
	if (m_shadercount >= 2)
	{
		// Perform the linking process
		glLinkProgram(m_shaderProgramHandle);
		// Check the status
		GLint linkStatus;
		glGetProgramiv(m_shaderProgramHandle, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			std::cout << "Shader program linking failed." << std::endl;
		//	glfwTerminate();
		}
		else
		{
			std::cout << "Shader program linking OK." << std::endl;
		}
	}
	else
	{
		std::cout << "Can't link shaders - you need at least 2, but attached shader count is only: " << std::to_string(m_shadercount)<< std::endl;
		glfwTerminate();
	}
}
    
GLuint ShaderProgram::getShaderProgramHandle()
{
	return m_shaderProgramHandle;
}

void ShaderProgram::use()
{
	glUseProgram(m_shaderProgramHandle);
}

ShaderProgram* ShaderProgram::update(std::string name, bool value) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform1i(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), value);
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, int value) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform1i(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), value);
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, float value) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform1f(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), value);

	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, double value) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform1f(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), value);
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::ivec2 vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform2iv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::ivec3 vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform3iv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::ivec4 vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform4iv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::vec2 vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform2fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::vec3 vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform3fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::vec4 vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform4fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, glm::value_ptr(vector));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::mat2 matrix) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniformMatrix2fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::mat3 matrix) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniformMatrix3fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, glm::mat4 matrix) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, std::vector<glm::vec2> vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform2fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), sizeof(vector), glm::value_ptr((&vector[0])[0]));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, std::vector<glm::vec3> vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform3fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), sizeof(vector), glm::value_ptr((&vector[0])[0]));
	return this;
}

ShaderProgram* ShaderProgram::update(std::string name, std::vector<glm::vec4> vector) 
{
	glUseProgram(m_shaderProgramHandle);
	glUniform4fv(glGetUniformLocation(m_shaderProgramHandle, name.c_str()), sizeof(vector), glm::value_ptr((&vector[0])[0]));
	return this;
}
