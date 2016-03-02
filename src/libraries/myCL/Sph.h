#ifndef SPH_H
#define SPH_H

#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>

#include <GL/glew.h>
#include <GL/glcorearb.h>
#include <GL/buffer.h>

#include <Util/util.h>

#include <glm/glm.hpp>

class SPH
{
	public:

		std::vector<int> m_vbos; //0: position VBO, 1: lifetime VBO 2: density VBO
		
		std::vector<glm::vec4> m_posInit;		//want to have the start points reseting particles
		std::vector<glm::vec4> m_veloInit;
		
		//SPH parameters and Buffers
		float dt;
		float poly6;
		float spikyConst;
		float visConst;
		float smoothingLength;
		float rho0;
		float buoyancy;
		float lifeDeduction;
		int m_numParticles; //number of particles
		
		int p_vbo; //position VBO
		int life_vbo; //life VBO
		int dens_vbo; //density VBO
		int rndm_vbo; //rndm Sprite
		int alive_vbo; //particle vision
		
		std::vector<glm::vec4> pos;
		std::vector<glm::vec4> vel;
		std::vector<float> life;
		std::vector<float> rndmSprite;
		std::vector<float> isAlive;
		std::vector<int> aliveHelper;
		std::vector<int> neighbours;
		std::vector<int> counter;
		std::vector<float> density;
		std::vector<float> pressure;
		std::vector<float> mass;
		std::vector<glm::vec4> forceIntern;

		size_t array_size; //the size of our arrays num * sizeof(Vec4)
		size_t float_size;
		size_t int_size; //(m_numParticles)
		size_t extended_int_size; //size of particles amount * count of saved neighbours per particle

		//default constructor initializes OpenCL Context and chooses platform and device
		SPH(float delta, float radiush, float r0, int num);
		//default deconstructor releases OpenCL objects and frees device memory
		~SPH();

		//setup the data for the kernel (push data to gpu)
		void loadData();
		
		//update data on the gpu
		void updateVBOs();

		//render function
		void render();

		//initialize 1 of 4 modes
		void init(int mode=1);

		//setter
		void setBuoyancy(float f);
		void setLifeDeduction(float f);


		void neighboursearch();
		void densPressCalc();
		void sphCalc();
		void integration();

	private: 

	float pVarPoly(float h, float r);

	glm::vec4 pVarSpiky(float h, glm::vec4 p, glm::vec4 pn);

	float pVarVisc(float h, glm::vec4 p, glm::vec4 pn);

};

#endif //SPH_H