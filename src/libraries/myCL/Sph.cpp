#define PI 3.14159265359

#include "Sph.h"

SPH::SPH(float delta, float radius, float r0, int num)
{
	//normal sph
	m_numParticles = num;
	dt = delta; 
	smoothingLength = radius; 
	rho0 = r0;

	poly6 = 315/(64*PI*pow(smoothingLength,9));
	spikyConst = -45/(PI*pow(smoothingLength,6));
	visConst = 45/(PI*pow(smoothingLength,6));

	buoyancy = 1.0f;
	lifeDeduction = 0.25;

	printf("Constants: \n dt = %f \n smoothingLength = %f \n poly6 = %f \n spikyConst = %f \n visConst = %f \n", 
		   dt, smoothingLength, poly6, spikyConst, visConst);
	printf("Number of Particles = %d \n", m_numParticles);

	pos.resize(m_numParticles);
	vel.resize(m_numParticles);
	life.resize(m_numParticles);
	rndmSprite.resize(m_numParticles);
	isAlive.resize(m_numParticles);
	aliveHelper.resize(m_numParticles);
	neighbours.resize(m_numParticles*50);
	counter.resize(m_numParticles);
	density.resize(m_numParticles);
	pressure.resize(m_numParticles);
	mass.resize(m_numParticles);
	forceIntern.resize(m_numParticles);
	m_posInit.resize(m_numParticles);
	m_veloInit.resize(m_numParticles);
}

SPH::~SPH()
{
}

void SPH::init(int mode)
{
	if(mode < 1 || mode > 4)
		mode = 1;

	float x,y,z;
	float rand_x,rand_y,rand_z;

	switch (mode)
	{
	case 1: //chimney
		for (int i = 0; i <m_numParticles; i++)
		{
			x = rand_float(-0.125,0.125);
			z = rand_float(-0.125,0.125);
			y = rand_float(0.125,0.25);
			pos[i] = glm::vec4(x,y,z,1.0f);

			rand_x = rand_float(-0.2,0.2);
			rand_y = rand_float(0.3,2.5);
			rand_z = rand_float(-0.2,0.2);
			vel[i] = glm::vec4(rand_x,rand_y,rand_z,0);
		
			setBuoyancy(50.0f);
			setLifeDeduction(0.15);
		}
		break;

	case 2: // big chimney
		for (int i = 0; i <m_numParticles; i++)
		{
			x = rand_float(-0.225,0.225);
			z = rand_float(-0.225,0.225);
			y = rand_float(0,0.25);
			pos[i] = glm::vec4(x,y,z,1.0f);

			rand_x = rand_float(-0.2,0.2);
			rand_y = rand_float(0.3,2.5);
			rand_z = rand_float(-0.2,0.2);
			vel[i] = glm::vec4(rand_x,rand_y,rand_z,0);
		
			setBuoyancy(50.0f);
			setLifeDeduction(0.25);
		}
		break;

	case 3: //side
		for (int i = 0; i <m_numParticles; i++)
		{
			x = rand_float(-0.75,-0.5);
			z = rand_float(-0.125,0.125);
			y = rand_float(0.25,0.5);
			pos[i] = glm::vec4(x,y,z,1.0f);

			rand_x = rand_float(0.5,3);
			rand_y = rand_float(0.25,1.5);
			vel[i] = glm::vec4(rand_x,rand_y,0,0);
		
			setBuoyancy(1.5f);
			setLifeDeduction(0.25);
		}
		break;
	
	case 4: //two sources (chimney)
		for (int i = 0; i <m_numParticles; i++)
		{

			if(i % 2 == 0)
			{
			x = rand_float(-0.125,0.125);
			z = rand_float(-0.125,0.125);
			y = rand_float(-0.49,-0.25);
			pos[i] = glm::vec4(x,y,z,1.0f);

			rand_x = rand_float(-0.2,0.2);
			rand_y = rand_float(1.0,1.75);
			rand_z = rand_float(-0.2,0.2);

			vel[i] = glm::vec4(rand_x,rand_y,rand_z,0);
			}
			else if (i % 2 == 1)
			{
			x = rand_float(-0.125,0.125);
			z = rand_float(-0.125,0.125);
			y = rand_float(1.99,1.75);
			pos[i] = glm::vec4(x,y,z,1.0f);

			rand_x = rand_float(-0.2,0.2);
			rand_y = rand_float(1.5,3.5);
			rand_z = rand_float(-0.2,0.2);
			vel[i] = glm::vec4(rand_x,-rand_y,rand_z,0);

			setBuoyancy(25.0f);
			setLifeDeduction(0.25);
			}
		}
		break;
	
	case 5: // two sources (side)
		for (int i = 0; i <m_numParticles; i++)
		{
			if(i % 2 == 0)
			{
			x = rand_float(-0.9,-0.75);
			z = rand_float(-0.25,0.25);
			y = rand_float(-0.40,-0.10);
			pos[i] = glm::vec4(x,y,z,1.0f);
		 
			rand_x = rand_float(1.0,4.5);
			rand_y = rand_float(0.35,1.75);
			rand_z = rand_float(-0.2,0.2);
			
			vel[i] = glm::vec4(rand_x,rand_y,rand_z,0);
			}
			else if (i % 2 == 1)
			{
			x = rand_float(0.70,0.9);
			z = rand_float(-0.25,0.25);
			y = rand_float(-0.40,-0.10);
			pos[i] = glm::vec4(x,y,z,1.0f);

			rand_x = rand_float(1.1,4.5);
			rand_y = rand_float(0.7,4.5);
			rand_z = rand_float(0.0,0.3);
			vel[i] = glm::vec4(-rand_x,rand_y,rand_z,0);
		
		    setBuoyancy(1.5f);
		    setLifeDeduction(0.25);
			}
		}
		break;
	}

	for (int i = 0; i <m_numParticles; i++)
	{
		life[i] = rand_float(0.0f,1.0f);
		rndmSprite[i] = float(i % 10);
		density[i] = 0.0f;
		pressure[i] = 1.0f;
		mass[i] = 0.000025f;
		forceIntern[i] = glm::vec4(0,0,0,0);
		counter[i]=0;

		m_posInit[i] = pos[i];
		m_veloInit[i] = vel[i];
	
		//set x particles alive at the beginning to avoid explosions
		isAlive[i] = 0.0;
		aliveHelper[i] = 0;
		if(i < 50)
		{
		isAlive[i] = 1.0;
		aliveHelper[i] = 1;
		}
	}
	
}

void SPH::loadData()
{
	printf("LOAD DATA \n");

	m_numParticles = pos.size();
	array_size = m_numParticles * sizeof(glm::vec4);
	extended_int_size = m_numParticles * sizeof(int) * 50;
	int_size = m_numParticles * sizeof(int);
	float_size = m_numParticles * sizeof(float);
	
	//create VBO's (util.cpp)
	p_vbo = createVBO(&pos[0], array_size, 4, 0); 
	life_vbo = createVBO(&life[0], float_size, 1,1);
	dens_vbo = createVBO(&density[0], float_size, 1, 2);
	rndm_vbo = createVBO(&rndmSprite[0], float_size, 1, 3);
	alive_vbo = createVBO(&isAlive[0], float_size, 1,4);

	m_vbos.push_back(p_vbo);
	m_vbos.push_back(life_vbo);
	m_vbos.push_back(dens_vbo);
	m_vbos.push_back(rndm_vbo);
	m_vbos.push_back(alive_vbo);

	printf("######################################################\n");
}

void SPH::neighboursearch()
{	
	for(int i = 0; i < m_numParticles; i++)
	{
		if (isAlive[i] > 0.5)
		{

		glm::vec4 p = pos[i];
	
		counter[i] = 0;
		//save neighbours of THIS particle in an array 
		//array size is 50 times bigger than pos[]
		for (int index = 0; index < m_numParticles; index++)
		{
			if (glm::distance(p, pos[index]) <= smoothingLength && isAlive[index] == 1) // < smoothingLength
			{
				neighbours[i*50+counter[i]] = index;
				counter[i]++;
				//only saves values with distance < smoothing Lenght --> [0,smoothingLength]
			}
			//stop when 50(n) neighbours of i are found
			if (counter[i] >= 49)
			break;
		}
		}
	}
}

void SPH::densPressCalc()
{
	for(int i = 0; i < m_numParticles; i++)
	{
		if (isAlive[i] > 0.5)
		{
	
		glm::vec4 p = pos[i];
		float rho = 0;
		float pressure_new = 0;
		float k = 0.00125; //Gaskonstante

		for(int index = 0; index < counter[i]; index++)
		{
			int j = neighbours[i * 50 + index];
			rho += pVarPoly(smoothingLength, glm::distance(p ,pos[j]));
		}
		rho *= poly6 * mass[i];

		density[i] = rho;
		pressure_new = k * (rho - rho0); //p = k * (rho-rho0)(k = stoffspezifische Konstante (Wasser 999kg/m³)) 

		pressure[i] = pressure_new;
	}
	}
}

void SPH::sphCalc()
{
	for(int i = 0; i < m_numParticles; i++)
	{
		if (isAlive[i] > 0.5)
		{
		glm::vec4 p = pos[i];
		float mue = 0.0000005; //je größer desto mehr zusammenhalt
	
		glm::vec4 f_pressure = glm::vec4(0.0,0.0,0.0,0.0f);
		glm::vec4 f_viscosity = glm::vec4(0.0,0.0,0.0,0.0f);

		//force calculation
		for(int index = 0; index < counter[i]; index++)
		{
			int j = neighbours[i * 50 + index];
			//fpressure calculation
			f_pressure += (pressure[i] + pressure[j])/density[j] * pVarSpiky(smoothingLength, p, pos[j]);
	
			//fviscosity calculation
			f_viscosity +=  (vel[j] - vel[i])/density[j] * pVarVisc(smoothingLength,p, pos[j]);
		}

		f_pressure *= -1.0f  * 1/2 * mass[i] * spikyConst ;
	
		f_viscosity *=  mue * visConst * mass[i];
	
		forceIntern[i] = f_pressure +  f_viscosity;
		forceIntern[i] /= density[i];
	}
	}
}

void SPH::integration()
{
	for(int i = 0; i < m_numParticles; i++)
	{
		if(aliveHelper[i] == 1)
		isAlive[i] = 1.0;

		if (isAlive[i] > 0.5)
		{

		glm::vec4 p_old = pos[i];
		glm::vec4 v_old = vel[i];
		glm::vec4 p_new = p_old;
		glm::vec4 v_new = v_old;

		life[i] -= lifeDeduction*dt;
		if(life[i] <= 0)
		  {
		      p_old = m_posInit[i];
		      v_old = m_veloInit[i];
		      life[i] = 1.0;    
		  }	

		float b = 50;

		//float gravityForce = -9.81f * mass[i];
		float gravityForce = buoyancy * (density[i] - rho0) * -9.81f * mass[i];

		//apply intern forces and extern forces
		v_new.x = v_old.x + (forceIntern[i].x/mass[i]) * dt;
		v_new.y = v_old.y + ((forceIntern[i].y + gravityForce)/mass[i]) * dt;
		v_new.z = v_old.z + (forceIntern[i].z/mass[i]) * dt;

		//compute new position with computed velocity
		//p_new.xyz = p_old.xyz + v_new.xyz * dt ;
		p_new = p_old + v_new * dt ;

	//	float bDamp = -1.2;
		float bDamp = -0.9;
	
		//boundarys
		if(p_new.y < -0.5)
		{
			v_new.y *= bDamp;
			p_new.y = -0.5f;
		}	
	
		if(p_new.y > 2)
		{
			v_new.y *= bDamp ;
			p_new.y = 2;
		}

		if(p_new.x > 1)
		{
			v_new.x *= bDamp;
			p_new.x = 1;
		}

		if(p_new.x < -1.0)
		{
			v_new.x *= bDamp;
			p_new.x = -1.0f;
		}

		if(p_new.z > 1)
		{
			v_new.z *= bDamp;
			p_new.z = 1;
		}

		if(p_new.z < -1)
		{
			v_new.z *= bDamp;
			p_new.z = -1.f;
		}

		//global damping
		v_new *= 0.99999f;

	   //update the arrays with newly computed values
	   //pos[i].xyz = p_new.xyz;
	   //vel[i].xyz = v_new.xyz;

		 pos[i] = p_new;
		 vel[i] = v_new;
		 
		}

	}
}

void SPH::updateVBOs()
{
	glBindBuffer(GL_ARRAY_BUFFER,p_vbo);
	glBufferData(GL_ARRAY_BUFFER, array_size, &pos[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,life_vbo);
	glBufferData(GL_ARRAY_BUFFER, float_size, &life[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,dens_vbo);
	glBufferData(GL_ARRAY_BUFFER, float_size, &density[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,alive_vbo);
	glBufferData(GL_ARRAY_BUFFER, float_size, &isAlive[0], GL_DYNAMIC_DRAW);
}

void SPH::render()
{
	//render Particles from VBOS
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_PROGRAM_POINT_SIZE);
	
	glDrawArrays(GL_POINTS, 0, m_numParticles);
}

void SPH::setBuoyancy(float b)
{
	buoyancy = b;
}

void SPH::setLifeDeduction(float f)
{
	lifeDeduction = f;
}

float SPH::pVarPoly(float h, float r)
{
	if (0<= r && r <= h)
		return pow(h*h - r*r,3);
	else return 0;
}

glm::vec4 SPH::pVarSpiky(float h, glm::vec4 p, glm::vec4 pn)
{
	glm::vec4 r = p - pn;
	float r_length = length(r);
	if (0< r_length && r_length <= h)
		return  r/r_length * pow(h - r_length,2);
	else return glm::vec4(0,0,0,0);
}

float SPH::pVarVisc(float h, glm::vec4 p, glm::vec4 pn)
{
	float r_length = length(p-pn);
	if (0<= r_length && r_length <= h)
		return h - r_length;
	else return 0;
}
