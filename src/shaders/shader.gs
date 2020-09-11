#version 430 core
layout (points) in;
layout (points, max_vertices = 100) out;
out vec4 ColorVec;

uniform int time;

in TIME{
	float a_time;
}the_time[];

void main(){
	gl_PointSize = 4.0f;
	float x = gl_in[0].gl_Position.x;
	float y = gl_in[0].gl_Position.y;
	float r = 1;
	float b = 1;
	float fade = .98;
	int index = int(the_time[0].a_time);
	
	for(int i = index; i >= 0; i--){
		gl_Position = vec4(x, y, 0.0, 1.0);
		ColorVec = vec4(r, 0, b, 1.0f);
		EmitVertex();
		r *= fade;
		b *= fade;
		x -= 0.001f * (x + y);//(cos(4 * ((x*x) + (y*y))));
		y -= 0.001f * (y - x);//((y*y) - (x*x));
	}
	EndPrimitive();
}