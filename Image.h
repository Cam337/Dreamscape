#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>

class Image {
private:
public:
	static unsigned char* loadImage(std::string fileLocation, int* width, int* height, int* nrChannels);
	static void freeImage(unsigned char* data);
};

#endif
