#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
	 
uniform	mat4 pr_matrix;
uniform	mat4 vw_matrix;
uniform mat4 ml_matrix = mat4(1.0f);

void main(){
	gl_Position = pr_matrix * vw_matrix * ml_matrix * vec4(position, 1.0f);
}