#version 330 core

layout (location = 0) in vec3 position;

// Uniform variables
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 cameraPosition;
uniform vec3 lightPosition; 

// Outputs to fragment shader
out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;

const float tiling = 4.0f;

void main()
{
	vec4 worldPosition = model * vec4(position.x, 0.0, position.y, 1.0);
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    //gl_Position = projection * view * worldPosition;
	clipSpace = projection * view * model * vec4(position, 1.0);
	gl_Position = clipSpace;
    textureCoords = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5) * tiling;
	toCameraVector = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;
}