#version 430 core

out vec4 fragColor;

in vec2 TexCoords;

uniform bool blackAndWhite;

uniform sampler2D screenTexture;

void main(){

	fragColor = vec4(vec3(texture(screenTexture, TexCoords)), 1.0f);

	if (blackAndWhite){
		float average = 0.2126f * fragColor.r + 0.7152f * fragColor.g + 0.0722 * fragColor.b;
		fragColor = vec4(average, average, average, 1.0f);
	}
}