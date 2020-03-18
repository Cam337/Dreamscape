#version 330 core

// Inputs from vertex shader
in vec3 textureCoords;

//uniform sampler2D reflectionTexture;
//uniform sampler2D refractionTexture;

// Outputs. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
	//vec4 reflectColor = texture(reflectionTexture, textureCoords);
	//vec4 refractColor = texture(refractionTexture, textureCoords);
	
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    // fragColor = mix(reflectColor, refractColor, 0.5);
	fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	
}