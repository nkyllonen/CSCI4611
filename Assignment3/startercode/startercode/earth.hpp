#ifndef EARTH_HPP
#define EARTH_HPP

#include "engine.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "util.h"

using namespace std;

using glm::vec2;
using glm::vec3;

float getV3Magnitude(vec3 v);

class Earth {
public:
    void initialize(Engine *engine, int slices, int stacks, float spherical);
    float isSpherical();
    void setSpherical(float spherical);
    vec3 getPosition(float latitude, float longitude);
    vec3 getNormal(float latitude, float longitude);
	vec2 getTCoord(float latitude, float longitude);
    void draw(bool textured);
protected:
    int slices, stacks;
    int nVertices, nTriangles;
    float spherical;
    Engine *engine;

    // TODO: Define the necessary buffers and texture.
    // Feel free to add helper methods to update the buffers.
	VertexBuffer vertexBuffer, normalBuffer, texCoordBuffer;
	ElementBuffer indexBuffer;
	Texture texture;
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<int> indices;
	vector<vec2> texCoords;

	//dimensions of the rectangular view
	float tWidth = 6;
	float tHeight = 3;

	void populateVNTArrays();
	void populateIArray();
};

inline void Earth::initialize(Engine *e, int sl, int st, float sp) {
    engine = e;
    slices = sl;
    stacks = st;
    spherical = sp;

    // TODO: Initialize nVertices, nTriangles, buffers, texture
	nVertices = (slices + 1)*(stacks + 1);
	nTriangles = 2 * slices*stacks;

	//load in texture
	texture = engine->loadTexture(Config::textureFile);

	//fill out vertices, normals, and indices
	populateVNTArrays();
	populateIArray();

	vertexBuffer = engine->allocateVertexBuffer(nVertices * sizeof(vec3));
	normalBuffer = engine->allocateVertexBuffer(nVertices * sizeof(vec3));
	indexBuffer = engine->allocateElementBuffer(indices.size() * sizeof(int));
	texCoordBuffer = engine->allocateVertexBuffer(nVertices * sizeof(vec2));

	//copy data over to buffers
	engine->copyVertexData(vertexBuffer, &vertices[0], nVertices * sizeof(vec3));
	engine->copyVertexData(normalBuffer, &normals[0], nVertices * sizeof(vec3));
	engine->copyElementData(indexBuffer, &indices[0], indices.size() * sizeof(int));
	engine->copyVertexData(texCoordBuffer, &texCoords[0], nVertices * sizeof(vec2));
}
/*-------------------------------------------------
	populateVNTArrays: fill vertices, normals,
						and texCoords arrays
-------------------------------------------------*/
inline void Earth::populateVNTArrays() {
	float xStep = 360.0 / slices;
	float yStep = 180.0 / stacks;

	//left to right, top to bottom
	for (float y = 90; y >= -90; y -= yStep) {
		for (float x = -180; x <= 180; x += xStep) {
			vertices.push_back(getPosition(y, x)); //(lat, long)
			normals.push_back(getNormal(y, x)); //(lat, long)
			texCoords.push_back(getTCoord(y, x)); //(lat, long)
		}
	}
}
/*-------------------------------------------------
	populateIArray: fill indices array
-------------------------------------------------*/
inline void Earth::populateIArray() {
	int topInd = 0, botInd = 0;

	for (int y = 0; y < stacks; y++) { //row by row
		for (int x = 0; x < slices ; x++) { //step down each row
			topInd = y*(slices + 1) + x;
			botInd = (y+1)*(slices + 1) + x;

			//bottom triangle
			indices.push_back(topInd);
			indices.push_back(botInd);
			indices.push_back(botInd + 1);

			//top triangle
			indices.push_back(topInd);
			indices.push_back(botInd + 1);
			indices.push_back(topInd + 1);
		}
	}
}

inline float Earth::isSpherical() {
    return spherical;
}
/*-------------------------------------------------
	setSpherical: update V and N buffers
					to display mesh as a sphere
-------------------------------------------------*/
inline void Earth::setSpherical(float s) {
    spherical = s;

    // TODO: Also update the relevant buffers.
    // You should only need to update two buffers, not all of them.
	//empty and repopulate V,N,T vectors
	vertices = vector<vec3>();
	normals = vector<vec3>();
	texCoords = vector<vec2>();
	populateVNTArrays();

	engine->copyVertexData(vertexBuffer, &vertices[0], nVertices * sizeof(vec3));
	engine->copyVertexData(normalBuffer, &normals[0], nVertices * sizeof(vec3));
}
/*-------------------------------------------------
	getPosition: returns vec3 representing 3D
					position at	corresponding
					lat and long
	(assume that the input latitude lies between
		−90° to 90°, and longitude lies between
		−180° to 180°)
-------------------------------------------------*/
inline vec3 Earth::getPosition(float latitude, float longitude) {
    vec3 rectangularPosition(0,0,0), sphericalPosition(0,0,0);

    // TODO compute vertex positions on rectangle and sphere
	rectangularPosition.x = (tWidth / 360.0)*longitude;
	rectangularPosition.y = (tHeight / 180.0)*latitude;

	//convert lat and long to radians for trig functions
	latitude = (latitude * M_PI) / 180.0;
	longitude = (longitude * M_PI) / 180.0;

	sphericalPosition.x = cos(latitude)*sin(longitude);
	sphericalPosition.y = sin(latitude);
	sphericalPosition.z = cos(latitude)*cos(longitude);

    if (spherical == 0)
        return rectangularPosition;
    else if (spherical == 1)
        return sphericalPosition;
    else {

        // TODO compute the interpolated position
        return vec3(0,0,0);

    }
}
/*-------------------------------------------------
	getNormal: returns normal vec3 corresponding
					to given latitude and longitude
	(assume that the input latitude lies between
		−90° to 90°, and longitude lies between
		−180° to 180°)
-------------------------------------------------*/
inline vec3 Earth::getNormal(float latitude, float longitude) {
    vec3 rectangularNormal(0,0,0), sphericalNormal(0,0,0);

    // TODO compute vertex positions on rectangle and sphere
	rectangularNormal.z = 1;//assuming z is pointing out towards the camera

	//normal is the radial vector normalized
	sphericalNormal = getPosition(latitude, longitude) - vec3(0, 0, 0);
	sphericalNormal /= Util::getV3Magnitude(sphericalNormal);

    if (spherical == 0)
        return rectangularNormal;
    else if (spherical == 1)
        return sphericalNormal;
    else {

        // TODO compute the interpolated normal
        return vec3(0,0,0);

    }
}
/*-------------------------------------------------
	getTCoord: returns texCoord corresponding to
				given latitude and longitude
	(assume that the input latitude lies between
		−90° to 90°, and longitude lies between
		−180° to 180°)
-------------------------------------------------*/
inline vec2 Earth::getTCoord(float latitude, float longitude) {
	vec2 tCoord(0, 0);

	//x --> longitude
	tCoord.x = (longitude / 360.0) + 0.5;
	//y --> latitude
	tCoord.y = -1.0*(latitude / 180.0) + 0.5;

	return tCoord;
}
inline void Earth::draw(bool textured) {
	engine->setVertexArray(vertexBuffer);
	engine->setNormalArray(normalBuffer);

    // TODO: Draw the mesh (with or without texture, depending on the input)
	if (textured) {
		engine->setTexCoordArray(texCoordBuffer);
		engine->setTexture(texture);
		engine->drawElements(GL_TRIANGLES, indexBuffer, indices.size());

		//so texture does not get applied to things drawn afterward
		engine->unsetTexture();
	}
	else { //draw mesh triangles instead
		glLineWidth(2);
		engine->drawElements(GL_TRIANGLES, indexBuffer, indices.size());
	}
}

#endif
