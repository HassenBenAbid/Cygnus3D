#version 430 core

struct Material {
	
	sampler2D diffuse;
	sampler2D specular;
	float shininess;

};

struct DirLight {
	
	vec3 direction;

	vec3 diffuse;
	vec3 specular;
};

struct PointLight {

	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 diffuse;
	vec3 specular;

};

struct SpotLight {
	
	vec3 diffuse;
	vec3 specular;

	float linear;
	float constant;
	float quadratic;

	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

};

layout (location = 0) out vec4 fragColor;

uniform vec3 viewPosition;

uniform Material material;

uniform vec3 ambientLight;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);
uniform bool singleColor = false;


in DATA{
	vec3 pos;
	vec3 normal;
	vec2 texCoord;
	vec3 fragPos;
	vec3 color;
}dataIn;

vec3 calcDirLight(vec3 diffuseSample, vec3 specularSample, float shininess, DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(vec3 diffuseSample, vec3 specularSample, float shininess, PointLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(vec3 diffuseSample, vec3 specularSample, float shininess, SpotLight light, vec3 normal, vec3 viewDir);

void main(){
	if (!singleColor){
		vec3 norm = normalize(dataIn.normal);
		vec3 viewDir = normalize(viewPosition - dataIn.fragPos);
		vec4 diffuseTexSample = texture(material.diffuse, dataIn.texCoord);
		vec4 specularTexSample = texture(material.specular, dataIn.texCoord);
	
		vec3 ambient = vec3(0.2f, 0.2f, 0.2f) * vec3(diffuseTexSample);
		vec3 result = ambient;
		
		result += calcDirLight(vec3(diffuseTexSample), vec3(specularTexSample), material.shininess, dirLight, norm, viewDir);
		result += calcPointLight(vec3(diffuseTexSample), vec3(specularTexSample), material.shininess, pointLight, norm, viewDir);
	
		result = vec3(diffuseTexSample);
	
		if (diffuseTexSample.a < 0.1f) discard;
		//if (result == vec3(0, 0, 0)) result = vec3(1.0f, 1.0f, 1.0f);
	
		fragColor = vec4(result * color, diffuseTexSample.a);
	
	}else{

	fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	}

}

float getAttenuation(vec3 lightPosition, float linear, float quadratic, float constant){

	
	float lightDistance = length(lightPosition - dataIn.fragPos);

	return 1.0f / (constant + linear * lightDistance + quadratic * (lightDistance * lightDistance));

}

vec3 calcDirLight(vec3 diffuseSample, vec3 specularSample, float shininess, DirLight light, vec3 normal, vec3 viewDir){

	vec3 lightDirection = normalize(-light.direction);

	//diffuse:
	vec3 diffuse = max(dot(normal, lightDirection), 0.0f) * diffuseSample * light.diffuse;

	//specular:
	vec3 reflectDir = reflect(-lightDirection, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0f), shininess) * specularSample * light.specular;

	return (diffuse + specular) ;

}

vec3 calcPointLight(vec3 diffuseSample, vec3 specularSample, float shininess, PointLight light, vec3 normal, vec3 viewDir){
	
	vec3 lightDirection = normalize(light.position - dataIn.fragPos);

	float attenuation = getAttenuation(light.position, light.linear, light.quadratic, light.constant);

	//diffuse:
	vec3 diffuse = (max(dot(normal, lightDirection), 0.0f) * diffuseSample) * light.diffuse;
	diffuse *= attenuation;

	//specular: 
	vec3 reflectDir = reflect(-lightDirection, normal);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0f), shininess) * specularSample * light.specular;
	specular *= attenuation;

	return (diffuse + specular);

}