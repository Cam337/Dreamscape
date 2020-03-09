#ifndef _TRACK_H_
#define _TRACK_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Geometry.h"
#include "BezierCurve.h"
#include "AnchorPoint.h"
#include "TangentPoint.h"
#include "ControlHandle.h"

class Track //: public Geometry
{
private:
	int N; // number of bezier curve segments
	std::vector<BezierCurve*> curves;
	std::vector<AnchorPoint*> anchorPoints;
	std::vector<TangentPoint*> tangentPoints;
	std::vector<ControlHandle*> controlHandles;
public:
	Track();
	~Track();

	void draw(GLuint shaderProgram);
	void update(glm::mat4 transform);
	void makeC1Continuous();

	std::vector<BezierCurve*> getCurves();

};
#endif

