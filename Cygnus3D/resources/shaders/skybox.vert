#version 430 core

layout (location = 0) in vec3 pos;

layout (std140, binding = 0) uniform global {
	 
	mat4 pr_matrix;
	
};

uniform mat4 vw_matrix;

out vec3 texCoord;


void main(){

	texCoord = pos;
	vec4 pos = pr_matrix * vw_matrix * vec4(pos, 1.0f);
	gl_Position = pos.xyww;

}