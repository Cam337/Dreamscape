#include "Track.h"

Track::Track()
{
	// Make 5 bezier curves, need 3N+1 points(16 for this track)
	// p0 and p3 are anchor points
	// p1 and p2 are tangent points
	// for each bezier curve, make control point p3 of previous equal to the p0 of the next, loop last p3 to first p0
	//std::cerr << "Track Constructor" << std::endl;

	N = 5;
	std::vector<glm::vec4> points;
	int numPoints = 3 * N + 1;

	// construct initial control points
	for (int i = 0; i < numPoints; i++)
	{
		double x, y, z;
		double degrees = (360.0 / (double)numPoints) * (double)i;

		x = cos(glm::radians(degrees)) * 10.0f;
		y = (rand() % 100) / 100.0 * 10.0f;
		z = sin(glm::radians(degrees)) * 10.0f;
		//std::cerr << "y = " << y << std::endl;

		glm::vec4 point(x,y,z,1.0f);

		points.push_back(point);

		if (i % 3 == 0)
		{
			anchorPoints.push_back(new AnchorPoint(points[i]));
		}
		else
			tangentPoints.push_back(new TangentPoint(points[i]));

	}

	// constrain tangent points to enfroce C1 continuity
	for (int i = 1; i < numPoints; i += 3)
	{
		if (i == 1)
		{
			points[i] = (2.0f*points[0]) - points[numPoints - 2];
		}
		else 
		{
			points[i] = (2.0f*points[i - 1]) - points[i - 2];
		}
	}

	// track anchor and tangent points
	for (int i = 0; i < numPoints; i++)
	{
		if (i % 3 == 0)
		{
			anchorPoints.push_back(new AnchorPoint(points[i]));
		}
		else
			tangentPoints.push_back(new TangentPoint(points[i]));
	}
	points[numPoints - 1] = points[0]; // the last point is equal to the first since we are making a looping closed track
	anchorPoints.pop_back();
	anchorPoints.push_back(anchorPoints[0]);

	//std::cerr << points.size() << " points created! is it 25?" << std::endl;
	makeC1Continuous();

	// construct each cubic bezier curve
	for (int i = 0; i < N; i++)
	{
		int index = i * 3;
		std::vector<glm::vec4> controlPoints;
		controlPoints.push_back(points[index]);
		controlPoints.push_back(points[index + 1]);
		controlPoints.push_back(points[index + 2]);
		controlPoints.push_back(points[index + 3]);
		BezierCurve* curve = new BezierCurve(controlPoints);
		curve->addAnchorPoint(anchorPoints[i]);
		curve->addAnchorPoint(anchorPoints[i+1]);
		curve->addTangentPoint(tangentPoints[i]);
		curve->addTangentPoint(tangentPoints[i+1]);
		curves.push_back(curve);
	}

	//std::cerr << curves.size() << " bezier curves created! is it 8?" << std::endl;


	// Control Handles
	AnchorPoint* ap = anchorPoints[0];
	TangentPoint* tp1 = tangentPoints[tangentPoints.size() - 1];
	TangentPoint* tp2 = tangentPoints[0];

	ControlHandle* ch = new ControlHandle(ap, tp1, tp2);
	controlHandles.push_back(ch);
	for (int i = 0; i < N-1; i++)
	{
		ap = anchorPoints[i+1];
		tp1 = tangentPoints[2*i+1];
		tp2 = tangentPoints[2*i+2];
		
		ch = new ControlHandle(ap, tp1, tp2);
		controlHandles.push_back(ch);
	}
}

Track::~Track()
{

}

void Track::draw(GLuint shaderProgram)
{
	//std::cerr << "Draw Track" << std::endl;

	for (BezierCurve* curve : curves)
	{
		curve->draw(shaderProgram);
	}

	
	/*
	for (AnchorPoint* anchorPoint : anchorPoints)
	{
		anchorPoint->draw(shaderProgram);
	}

	for (TangentPoint* tangentPoint : tangentPoints)
	{
		tangentPoint->draw(shaderProgram);
	}

	for (ControlHandle* ch : controlHandles)
	{
		ch->draw(shaderProgram);
	}
	*/
}

void Track::update(glm::mat4 transform)
{
	for (BezierCurve* curve : curves)
	{
		curve->update(transform);
	}

	for (AnchorPoint* anchorPoint : anchorPoints)
	{
		anchorPoint->update(transform);
	}

	for (TangentPoint* tangentPoint : tangentPoints)
	{
		tangentPoint->update(transform);
	}

	for (ControlHandle* ch : controlHandles)
	{
		ch->update(transform);
	}
}

void Track::makeC1Continuous()
{

}

std::vector<BezierCurve*> Track::getCurves()
{
	return curves;
}
