#version 430 core
layout (points) in;
layout (points, max_vertices = 256) out;
out vec4 ColorVec;
in TRANSFER{
	float time;
	float fade;
}transfer[];

uniform float Scale;
uniform float currX;
uniform float currY;
uniform float scalingFactor;
uniform float pixelSize;

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
	float r = transfer[0].fade;
	float b = transfer[0].fade;
	float fade = .98;
	int index = int(transfer[0].time);
	
	// trail generation
    for(int i = index; i >= 0; i--){
		gl_Position = scaler * vec4(
            x + (currX * (1 / tempScale)),
            y + (currY * (1 / tempScale)),
            0.0,
            1.0
        );
		ColorVec = vec4(r, 0, b, 1.0f);
		EmitVertex();
		r *= fade;
		b *= fade;
		x -= scalingFactor * (X_EQUATION);
		y -= scalingFactor * (Y_EQUATION);
	}
	EndPrimitive();
}
