#include "Texture.h"

Texture::Texture(std::string path)
{
	m_textureHandle = load(path);
}

Texture::~Texture()
{
    glDeleteTextures(1,&m_textureHandle);
}

GLuint Texture::getHandle()
{
	return m_textureHandle;
}

GLuint Texture::load(std::string path)
{
	int bytesPerPixel = 0;

	unsigned char *data = stbi_load(path.c_str(), &m_width, &m_height, &bytesPerPixel, 0);

	//flip image vertically
	unsigned char* s = data;
	for (int y=0; y<m_height/2; y++)
	{
		unsigned char* e = data+(m_height-y-1)*m_width*bytesPerPixel;
		for(int x=0; x<m_width*bytesPerPixel; x++)
		{
			unsigned char temp = *s;
			*s = *e;
			*e = temp;
			s++;
			e++;
		}
	}

	//send image data to the new texture
	if (bytesPerPixel < 3)
	{
		printf("ERROR: Unable to load texture image %s\n", path);
		return -1;
	}
	else 
	{
		setTexture( m_width, m_height, bytesPerPixel, data);
	}

	//stbi_image_free(data);	//keep copy e.g. for CPU ray tracing
	glGenerateMipmap(GL_TEXTURE_2D);          
	return m_textureHandle; 
}

void Texture::setTexture( int width, int height, int bytesPerPixel, unsigned char *data)
{
	m_width = width;
	m_height = height;
	m_bytesPerPixel = bytesPerPixel;
	m_data = data;


	glGenTextures( 1, &m_textureHandle);
	glBindTexture( GL_TEXTURE_2D, m_textureHandle);
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );


	glBindTexture( GL_TEXTURE_2D, m_textureHandle);

	if (m_bytesPerPixel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
	} 
	else if (m_bytesPerPixel == 4) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	} 
	else 
	{
		printf("RESOLVED: Unknown format for bytes per pixel in texture, changed to 4\n");
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	}
	glGenerateMipmap(GL_TEXTURE_2D);    
}
