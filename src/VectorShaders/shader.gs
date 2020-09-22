#version 430 core
layout (points) in;
layout (points, max_vertices = 100) out;
out vec4 ColorVec;
in TIME{
	float a_time;
}the_time[];
/*--------------------------------------------------------------------------------------------------------

Uniforms

--------------------------------------------------------------------------------------------------------*/
uniform float Scale;
uniform float currX;
uniform float currY;
uniform float scalingFactor;
uniform float pixelSize;
/*--------------------------------------------------------------------------------------------------------

Main Function

--------------------------------------------------------------------------------------------------------*/
void main(){
	float tempScale = 1 / Scale;
	
	mat4 scaler;
	scaler[0] = vec4(tempScale, 0, 0, 0);
	scaler[1] = vec4(0, tempScale, 0, 0);
	scaler[2] = vec4(0, 0, tempScale, 0);
	scaler[3] = vec4(0, 0, 0, 1);

	gl_PointSize = pixelSize * (tempScale);
	float x = gl_in[0].gl_Position.x;
	float y = gl_in[0].gl_Position.y;
	float r = 1;
	float b = 1;
	float fade = .975;
	int index = int(the_time[0].a_time);
	
/*--------------------------------------------------------------------------------------------------------

Trail Generation

--------------------------------------------------------------------------------------------------------*/
	for(int i = index; i >= 0; i--){
		gl_Position = scaler * vec4(x + (1.75f * currX * (1 / tempScale)), y + (1.75f * currY * (1 / tempScale)), 0.0, 1.0);
		ColorVec = vec4(r, 0, b, 1.0f);
		EmitVertex();
		r *= fade;
		b *= fade;
		x -= scalingFactor * (x + y);
		y -= scalingFactor * (y - x);
	}
	EndPrimitive();
}
