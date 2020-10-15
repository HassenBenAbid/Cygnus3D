#version 420 core

#define MAX_LIGHTS 50

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

out vec4 fragColor;

struct Material {
	
	vec4 diffuseColor;
	vec4 specularColor;
	vec4 emissiveColor;

	float opacity;
	float specularPower;
	float specularScale;
	float alphaThreshold;

	float bumpIntensity;

	bool hasDiffuseTexture;
	bool hasSpecularTexture;
	bool hasNormalTexture;
	bool hasOpacityTexture;
	bool hasEmissiveTexture;

};

struct Light{

	vec4 color;

	vec3 position;
	float range;
	
	vec3 direction;
	float intensity;
	
	float spotLightAngle;
	bool enabled;
	int type;
	
	float PADDING;

};

struct LightingResult{
	vec4 diffuse;
	vec4 specular;
};

uniform	sampler2D diffuseTexture;
uniform	sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform sampler2D opacityTexture;
uniform	sampler2D emissiveTexture;

uniform vec4 globalAmbient;
uniform vec3 viewPosition;

uniform Material material;

uniform int lightNumber;
//uniform Light allLights[MAX_LIGHTS];

layout (std140, binding = 1) uniform lightBlock{

	Light allLights[MAX_LIGHTS];

};

in DATA{
	vec3 pos;
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
	vec3 color;
}dataIn;

LightingResult doLighting(Light lights[MAX_LIGHTS], Material mat, vec3 viewPos, vec3 position, vec3 normal);
void main(){

	vec4 diffuse = material.diffuseColor;
	if (material.hasDiffuseTexture){
		
		vec4 diffuseTex = texture(diffuseTexture, dataIn.texCoord.xy);
		if (diffuse.xyz != vec3(0)){
			diffuse *= diffuseTex;
		}else {
			diffuse = diffuseTex;
		}
		
	}
	
	float alpha = diffuse.a;
	if (material.hasOpacityTexture){
		alpha = texture(opacityTexture, dataIn.texCoord).r;
	}
	
	vec4 ambient = globalAmbient * diffuse;
	
	vec4 emissive = material.emissiveColor;
	if (material.hasEmissiveTexture){
	
		vec4 emissiveTex = texture(emissiveTexture, dataIn.texCoord);
		if (emissive.xyz != vec3(0)){
			emissive *= emissiveTex;
		}else {
			emissive = emissiveTex;
		}
	}
	
	vec3 norm = normalize(dataIn.normal);
	
	LightingResult endLight = doLighting(allLights, material, viewPosition, dataIn.fragPos, norm);
	
	
	diffuse *= endLight.diffuse;
	
	vec4 specular = vec4(0.0f);
	if (material.specularPower > 1.0f){
	
		specular = material.specularColor;
	
		if (material.hasSpecularTexture){
			vec4 specTex = texture(specularTexture, dataIn.texCoord);
			if (specular.xyz != vec3(0)){
				specular *= specTex;
			}else {
				specular = specTex;
			}
		}
	
		specular *= endLight.specular;
	}
	
	fragColor = vec4((ambient + diffuse + specular).xyz, 1.0f);

}

vec4 doDiffuse(Light light, vec3 lightDirection, vec3 normal){
	
	return light.color * (max(dot(normal, lightDirection), 0));

}

vec4 doSpecular(Light light, Material mat, vec3 viewDirection, vec3 lightDirection, vec3 normal){

	vec3 reflection = normalize(reflect(-lightDirection, normal));
	float refDotView = max(dot(reflection, viewDirection), 0);

	return light.color * pow(refDotView, material.specularPower);

}

float doAttenuation(Light light, float distance){

	return 1.0f - smoothstep(light.range * 0.75f, light.range, distance);

}

LightingResult doDirectionalLight(Light light, Material mat, vec3 viewDirection, vec3 position, vec3 normal){

	LightingResult current;

	vec3 dirLight = normalize(-light.direction);

	current.diffuse = doDiffuse(light, dirLight, normal) * light.intensity;
	current.specular = doSpecular(light, mat, viewDirection, dirLight, normal) * light.intensity;

	return current;
}

LightingResult doPointLight(Light light, Material mat, vec3 viewDirection, vec3 position, vec3 normal){

	LightingResult current;

	vec3 dirLight = light.position - position;
	float distance = length(dirLight);
	dirLight /= distance;

	float attenuation = doAttenuation(light, distance);

	current.diffuse = doDiffuse(light, dirLight, normal) * attenuation * light.intensity;
	current.specular = doSpecular(light, mat, viewDirection, dirLight, normal) * attenuation * light.intensity;

	return current;

}

LightingResult doSpotLight(Light light, Material mat, vec3 viewDirection, vec3 position, vec3 normal){

	LightingResult current;

	vec3 lightDirection = light.position - position;
	float distance = length(lightDirection);
	lightDirection /= distance;

	float attenuation = doAttenuation(light, distance);

	float minCos = cos(radians(light.spotLightAngle));
	float maxCos = mix(minCos, 1, 0.5f);
	float cosAngle = dot(light.direction, -lightDirection);

	float spotIntensity = smoothstep(minCos, maxCos, cosAngle);

	current.diffuse = doDiffuse(light, lightDirection, normal) * attenuation * spotIntensity * light.intensity;
	current.specular = doSpecular(light, mat, viewDirection, lightDirection, normal) * attenuation * spotIntensity * light.intensity;

	return current;

}

LightingResult doLighting(Light lights[MAX_LIGHTS], Material mat, vec3 viewPos, vec3 position, vec3 normal){

	vec3 viewDirection = normalize(viewPos - position);

	LightingResult total;
	total.diffuse = vec4(0);
	total.specular = vec4(0);

	for(int i=0; i < MAX_LIGHTS; i++){

		if (!lights[i].enabled) continue;
		if (lights[i].type != DIRECTIONAL_LIGHT && (length(lights[i].position - position) > lights[i].range)) continue;
		
		LightingResult current;
		current.diffuse = vec4(0);
		current.specular = vec4(0);

		if (lights[i].type == DIRECTIONAL_LIGHT) current = doDirectionalLight(lights[i], mat, viewDirection, position, normal);
		else if (lights[i].type == POINT_LIGHT) current = doPointLight(lights[i], mat, viewDirection, position, normal);
		else if (lights[i].type == SPOT_LIGHT) current = doSpotLight(lights[i], mat, viewDirection, position, normal);
	
		total.diffuse += current.diffuse;
		total.specular += current.specular;
	}

	return total;

}