#version 330 core

// Inputs from vertex shader
in vec3 textureCoords;

uniform vec3 color;

// Outputs. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(0.0, 0.0, 1.0, 1.0);
	
}