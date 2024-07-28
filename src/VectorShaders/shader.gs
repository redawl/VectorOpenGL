#version 430 core
layout (points) in;
layout (points, max_vertices = 256) out;
out vec4 ColorVec;
in TRANSFER{
	float time;
	float fade;
}transfer[];

uniform float zoomLevel;
uniform float xOffset;
uniform float yOffset;
uniform float speedFactor;
uniform float pixelSize;

void main(){
	float scale = 1 / zoomLevel;
	
	mat4 transformer;
	transformer[0] = vec4(scale, 0, 0, 0);
	transformer[1] = vec4(0, scale, 0, 0);
	transformer[2] = vec4(0, 0, scale, 0);
	transformer[3] = vec4(0, 0, 0, 1);

	gl_PointSize = pixelSize / zoomLevel;
	float x = gl_in[0].gl_Position.x;
	float y = gl_in[0].gl_Position.y;
	float r = transfer[0].fade;
	float b = transfer[0].fade;
	float fade = .98;
	int index = int(transfer[0].time);
	
	// trail generation
    for(int i = index; i >= 0; i--){
		gl_Position = transformer * vec4(
            x + (xOffset * zoomLevel),
            y + (yOffset * zoomLevel),
            0.0,
            1.0
        );
		ColorVec = vec4(r, 0, b, 1.0f);
		EmitVertex();
		r *= fade;
		b *= fade;
		x -= speedFactor * (X_EQUATION);
		y -= speedFactor * (Y_EQUATION);
	}
	EndPrimitive();
}
