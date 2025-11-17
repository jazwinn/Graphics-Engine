#version 330 core

in vec3 crntPos;
in vec3 Normal;
in vec4 color;
in vec2 texCoord;


out vec4 FragColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor = vec4(1.0);;
uniform vec3 lightPos = vec3(0.0, 5.0, 0.0);;
uniform vec3 camPos;

uniform bool useTexture;

vec4 getDiffuseColor() {
    if (useTexture) {
        return texture(diffuse0, texCoord); // use texture
    } else {
        return vec4(1.0); // fallback to white
    }
}

float getSpecularValue() {
    if (useTexture) {
        return texture(specular0, texCoord).r; // use texture
    } else {
        return 1.0; // fallback to full specular
    }
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
	float ambient = 0.80f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
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
	
	vec4 light = directLight();

	FragColor = color * light;
}