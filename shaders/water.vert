#version 330 core

layout (location = 0) in vec3 position;

// Uniform variables
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// Outputs to fragment shader
out vec2 textureCoords;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    // gl_Position = projection * view * model * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = projection * view * model * vec4(position, 1.0);
	textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5);
    
}