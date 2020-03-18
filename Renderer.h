#ifndef _RENDERER_H_
#define _RENDERER_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "RawModel.h"

class Renderer 
{
private:
public:
	void prepare();
	void render(RawModel* model);

};

#endif
