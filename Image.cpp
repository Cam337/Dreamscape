#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* Image::loadImage(std::string fileLocation, int* width, int* height, int* nrChannels)
{
	return stbi_load(fileLocation.c_str(), width, height, nrChannels, 0);
}

void Image::freeImage(unsigned char* data)
{
	stbi_image_free(data);
}
