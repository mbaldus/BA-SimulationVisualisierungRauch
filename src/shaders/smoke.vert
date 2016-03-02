#version 330

 //!< in-variables
layout(location = 0) in vec4 positionAttribute;
layout(location = 1) in float lifeBuffer;
layout(location = 2) in float densityBuffer;
layout(location = 3) in float rndmS;
layout(location = 4) in float alive;

//!< out-variables

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 passPosition;
out float passLifeBuffer;
out float passDensityBuffer;
out float rndmSprite;
out float isAlive;

void main()
{
	gl_Position = projection * view * model * positionAttribute;
	passPosition = model * positionAttribute;

	gl_PointSize = (2-lifeBuffer) * 187.5 / gl_Position.w; 
	//2 - 1 = 1 * 187.5  = 187.5 am Anfangs
	//2 - 0 = 2 * 187.5	= 375	am Ende

	//passthroughs
	passLifeBuffer = lifeBuffer;
	passDensityBuffer = densityBuffer;
	rndmSprite = rndmS;
	isAlive = alive;
}