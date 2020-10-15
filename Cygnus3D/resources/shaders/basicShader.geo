#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in DATA{
	vec3 pos;
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
}dataIn[];

void main(){


	vec3 normal = getNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	TexCoord = dataIn[0].texCoord;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, normal);
	TexCoord = dataIn[1].texCoord;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, normal);
	TexCoord = dataIn[2].texCoord;
	EmitVertex();

	EndPrimitive();

}