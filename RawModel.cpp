#include "RawModel.h"

RawModel::RawModel(GLuint vaoID, int vertexCount) :
	vaoID(vaoID), vertexCount(vertexCount)
{

}

GLuint RawModel::getVaoID()
{
	return vaoID;
}

int RawModel::getVertexCount()
{
	return vertexCount;
}
