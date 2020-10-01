#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in float time;
layout (location = 2) in float fade;
out TRANSFER{
	float time;
	float fade;
}transfer;

void main()
{	
	transfer.fade = fade;
	transfer.time = time;
	gl_Position = aPos;
}