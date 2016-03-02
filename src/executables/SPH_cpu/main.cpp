#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_PARTICLES 8192

#include <myCL/Sph.h>
#include <Util/util.h>
#include <GL/GLTools.h>
#include <GL/CVK_Trackball.h>
#include <GL/ShaderProgram.h>
#include <GL/Texture.h>
 
void loadAndBindTextures(Texture* textures[], ShaderProgram* shaderprogram)
{
	textures[0] = new Texture(TEXTURES_PATH "/smoke10.png");
	textures[1] = new Texture(TEXTURES_PATH "/smoke1.png");
	textures[2] = new Texture(TEXTURES_PATH "/smoke2.png");
	textures[3] = new Texture(TEXTURES_PATH "/smoke3.png");
	textures[4] = new Texture(TEXTURES_PATH "/smoke4.png");
	textures[5] = new Texture(TEXTURES_PATH "/smoke5.png");
	textures[6] = new Texture(TEXTURES_PATH "/smoke6.png");
	textures[7] = new Texture(TEXTURES_PATH "/smoke7.png");
	textures[8] = new Texture(TEXTURES_PATH "/smoke8.png");
	textures[9] = new Texture(TEXTURES_PATH "/smoke9.png");
	
	shaderprogram->use();

	for (int i = 0; i < 10; i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->getHandle());
	}
}

int main(void) {
	printf("CPU VERSION SPH\n");
	
	GLFWwindow* window = GLTools::generateWindow(1280,720,100,100,"SPH Demo");
	glClearColor(0.85, 0.85, 0.85, 0.0);

	Trackball trackball(GLTools::getWidth(window),GLTools::getHeight(window));

	int num = NUM_PARTICLES;
	SPH* sph = new SPH(0.00375f,0.05f,0.59,num);
    
	//###################################################################
	//						SPH INITILIZATION
	  /*
	  mode 1 => chimney
	  mode 2 => big chimney
	  mode 3 => side
	  mode 4 => two sources chimney
	  mode 5 => two sources side
	  */
	sph->init(2);
	sph->loadData();

	//###################################################################
	//				GL ShaderProgram and Camera Settings
	ShaderProgram* shaderprogram = new ShaderProgram("/smoke.vert", "/smokeSprite.frag");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f,-0.1f,1.0f),glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 projection = glm::perspective(45.f, GLTools::getRatio(window), 0.1f, 100.f);
	shaderprogram->update("model",model);
	shaderprogram->update("view",view);
	shaderprogram->update("projection",projection);

	//						    Textures
	Texture* textures[10];
	loadAndBindTextures(textures, shaderprogram);
	GLint smokeTexs[10]={0,1,2,3,4,5,6,7,8,9}; //Array that Contains all Smoketextures for Uniform
	glUniform1iv(glGetUniformLocation(shaderprogram->getShaderProgramHandle(), "smokeTextures"), 10, smokeTexs);
	
	//###################################################################
	int framecount =0;
	int frameoffset= 20 ;

	float time_spent = 0.0f;
	float max_time_spent = 0.0f;
	float average_time = 0.0f;

	std::function<void(double)> loop = 
		[&sph,
		&shaderprogram,
		&trackball,
		&model,
		&view, &framecount, &frameoffset,
		&time_spent, &max_time_spent, &average_time,
		&window](double deltatime)
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		
		shaderprogram->use();
		trackball.update(window,view);
		shaderprogram->update("view", view);
		
		if (framecount * frameoffset <= NUM_PARTICLES)
		{
			for (int j = 0; j < frameoffset; j++)
			{
				if(framecount * frameoffset+j<NUM_PARTICLES)
				sph->aliveHelper[framecount * frameoffset+j] = 1 ;
			}
			printf("\rParticles alive: %d     ", framecount * frameoffset);
		}
	
		clock_t begin; //<Timer>
		begin = clock(); //<Timer>

		sph->neighboursearch();  
		sph->densPressCalc(); 
		sph->sphCalc();	 
		sph->integration(); 
		
		//Timer
		time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC;
		average_time += time_spent;

		if (time_spent > max_time_spent)
			max_time_spent = time_spent;

		if (framecount % 100 == 0)
		{
			printf("Average Simulation = %f sec (100 Steps) \n", average_time/=100);
		}
		//End Timer


		sph->updateVBOs();
		sph->render();

		framecount++;

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

	};
	GLTools::render(window, loop);
	
	printf("Maximal Simulation time: %f sec \n", max_time_spent);

	//cleanup
	GLTools::destroyWindow(window);
	delete shaderprogram;
	delete sph;
   return 0;
}

