#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 normalVector;

uniform vec3 color;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
	//vec3 normal = normalize(normalVector);
	//normal = normal + 1.0f;
	//normal = normal / 2.0f;

    fragColor = vec4(color, 1.0f);
}