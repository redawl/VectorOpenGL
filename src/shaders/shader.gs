#version 430 core
layout (points) in;
layout (points, max_vertices = 100) out;
out vec4 ColorVec;

uniform int time;

in TIME{
	float a_time;
}the_time[];
uniform float Scale;
uniform float currX;
uniform float currY;
uniform float scalingFactor;

void main(){
	float tempScale = Scale;
	if(tempScale <= 0){
		tempScale = 1;
	}
	mat4 scaler;
	scaler[0] = vec4(1 / tempScale, 0, 0, 0);
	scaler[1] = vec4(0, 1 / tempScale, 0, 0);
	scaler[2] = vec4(0, 0, 1 / tempScale, 0);
	scaler[3] = vec4(0, 0, 0, 1);

	gl_PointSize = 5.0f;
	float x = gl_in[0].gl_Position.x;
	float y = gl_in[0].gl_Position.y;
	float r = 1;
	float b = 1;
	float fade = .98;
	int index = int(the_time[0].a_time);
	
	for(int i = index; i >= 0; i--){
		gl_Position = scaler * vec4(x + currX, y + currY, 0.0, 1.0);
		ColorVec = vec4(r, 0, b, 1.0f);
		EmitVertex();
		r *= fade;
		b *= fade;
		x -= scalingFactor * /*(x + y);*/(cos(4 * ((x*x) + (y*y))));
		y -= scalingFactor * /*(y - x);*/((y*y) - (x*x));
	}
	EndPrimitive();
}