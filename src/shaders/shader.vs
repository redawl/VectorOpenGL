#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in float time;
out TIME{
	float a_time;
}the_time;

void main()
{	
	the_time.a_time = time;
	gl_Position = aPos;
}