#version 330 core

#define MAX_DIFFUSE 4
#define MAX_SPECULAR 4

in vec3 crntPos;
in vec3 Normal;
in vec4 color;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D diffuse[MAX_DIFFUSE];
uniform sampler2D specular[MAX_SPECULAR];
uniform int diffuseCount;
uniform int specularCount;

uniform vec4 lightColor = vec4(1.0);;
uniform vec3 lightPos = vec3(0.0, 5.0, 0.0);;
uniform vec3 camPos;
uniform int lightType;
uniform bool useTexture;

vec4 getDiffuseColor() {
    if (!useTexture || diffuseCount == 0) return vec4(1.0);

    vec4 result = vec4(0.0);
    for (int i = 0; i < diffuseCount; i++) {
        result += texture(diffuse[i], texCoord);
    }
    return result / float(diffuseCount);
}

float getSpecularValue() {
    if (!useTexture || specularCount == 0) return 1.0;

    float result = 0.0;
    for (int i = 0; i < specularCount; i++) {
        result = max(result, texture(specular[i], texCoord).r);
    }
    return result;
}



vec4 pointLight()
{	
	// used in two variables so I calculate it here to not have to do it twice
     vec3 lightVec = lightPos - crntPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float maxDist = 10.0;
	float inten = max(0.0, 1.0 - (dist / maxDist));
	inten = inten * inten; // Square for smoother falloff
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return getDiffuseColor() * ((diffuse * inten + ambient) * lightColor) + getSpecularValue() * specular * inten * lightColor;
}

vec4 directLight()
{
	// ambient lighting
	float ambient = 1.f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(0.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (getDiffuseColor() * (diffuse + ambient) + getSpecularValue() * specular) * lightColor;
}

vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (getDiffuseColor() * (diffuse * inten + ambient) + getSpecularValue() * specular * inten) * lightColor;
}



void main(){
	vec4 light;
	if(lightType == 0){
		light = pointLight();
	}
	else if(lightType == 1){
		
		light = directLight();
	}
	else if(lightType == 2){
		
		light = spotLight();
	}
	

	FragColor = color * light;
}