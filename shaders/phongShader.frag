#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform Light dirLight;
uniform Light light;
uniform bool normalColorMode;

void main()
{
	vec3 norm;
	vec3 lightDir;
	float diff;
	vec3 viewDir;
	vec3 reflectDir;
	float spec;

	// DIRECTIONAL LIGHT

	// ambient
    vec3 dirAmbient = dirLight.ambient * material.ambient;
  	
    // diffuse 
    norm = normalize(Normal);
    lightDir = normalize(-light.direction);
    diff = max(dot(norm, lightDir), 0.0);
    vec3 dirDiffuse = dirLight.diffuse * (diff * material.diffuse);
    
    // specular
    viewDir = normalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);  
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 dirSpecular = dirLight.specular * (spec * material.specular);

	// POINT LIGHT

    // ambient
    vec3 ambient = dirLight.ambient * material.ambient;

    // diffuse 
    norm = normalize(Normal);
    lightDir = normalize(light.position - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    viewDir = normalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);  
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (distance);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

    vec3 result = dirAmbient + dirDiffuse + dirSpecular + ambient + diffuse + specular;
	if(normalColorMode)
	{
		vec3 norm = normalize(Normal);
		norm = norm + 1.0f;
		norm = norm / 2.0f;
		FragColor = vec4(norm, 1.0f);
	}
	else
		FragColor = vec4(result, 1.0);
} 