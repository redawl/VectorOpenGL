#version 430 core
layout (location = 0) in vec4 aPos;
in float time;
out TIME{
	float a_time;
}the_time;

void main()
{	
	the_time.a_time = time;
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}