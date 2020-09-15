#version 420 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;


in DATA{
	vec3 pos;
	vec3 normal;
}dataIn[];

void generateLine(int index){
	
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = gl_in[index].gl_Position + vec4(dataIn[index].normal, 0.0) * 0.2f;
	EmitVertex();

	EndPrimitive();

}

void main(){
	
	generateLine(0);
	generateLine(1);
	generateLine(2);

}