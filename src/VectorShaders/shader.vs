#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in float time;
layout (location = 2) in float fade;
out TIME{
	float a_time;
}the_time;

out FADE{
	float a_fade;
}the_fade;

void main()
{	
	the_fade.a_fade = fade;
	the_time.a_time = time;
	gl_Position = aPos;
}