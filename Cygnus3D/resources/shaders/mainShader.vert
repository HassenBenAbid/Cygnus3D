#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in ivec3 boneID;
layout (location = 4) in vec3 boneWeight;

const int MAX_BONES = 100;

layout (std140, binding = 0) uniform global {
	 
	mat4 pr_matrix;
	mat4 vw_matrix;
	
};

uniform mat4 ml_matrix = mat4(1.0f);

uniform bool hasBones;
uniform mat4 bonesMatrix[MAX_BONES];

out DATA{
	vec3 pos;
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
	vec3 color;
}dataOut;

void main(){

	vec4 currentPosition = vec4(position, 1.0f);
	vec4 currentNormal = vec4(normal, 0.0f);

	if (hasBones){
		
		mat4 bonesTransformation = bonesMatrix[boneID[0]] * boneWeight[0];
		bonesTransformation += bonesMatrix[boneID[1]] * boneWeight[1];
		bonesTransformation += bonesMatrix[boneID[2]] * boneWeight[2];
		
		currentPosition = bonesTransformation * currentPosition;
		currentNormal = bonesTransformation * currentNormal;

	}

	gl_Position = pr_matrix * vw_matrix * ml_matrix * currentPosition;
	
	dataOut.pos = position;
	dataOut.normal = mat3(ml_matrix) * vec3(currentNormal);
	dataOut.texCoord = texCoord;

	dataOut.fragPos = vec3(ml_matrix * vec4(position, 1.0f));

}