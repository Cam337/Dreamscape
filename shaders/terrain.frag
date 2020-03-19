#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

/*
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool normColorMode;
*/
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform sampler2D grassTexture;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 FragColor;

void main()
{
  // ambient
  vec3 ambient = vec3(0.05f, 0.05f, 0.05f);

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPosition - FragPos);
  float diffuse = max(dot(norm, lightDir), 0.0);

  // specular
  vec3 viewDir = normalize(cameraPosition - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float specular = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);

  vec3 result = ambient + diffuse + specular;

  /*
  if(normColorMode) {
    norm = norm + 1;
    norm = norm / 2;
    result = norm;
  }
  */

  // Use the color passed in. An alpha of 1.0f means it is not transparent.
  //FragColor = vec4(result, 1.0f);
  //FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
  FragColor =  vec4(result, 1.0f) * texture(grassTexture, TexCoord);
}
