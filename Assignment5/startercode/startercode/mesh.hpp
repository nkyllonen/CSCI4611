#ifndef MESH_HPP
#define MESH_HPP

#include "engine.hpp"
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::ivec2;
using glm::ivec3;

class Mesh {
public:
    void loadOBJ(const std::string &filename);
    void createGPUData(Engine *engine);
    std::vector<vec3> vertices;   // vertex positions
    std::vector<vec3> colors;     // vertex colors
    std::vector<vec3> normals;    // vertex normals
    std::vector<vec2> texCoords;  // vertex texture coordinates
    std::vector<ivec3> triangles; // triangle vertex indices
    VertexBuffer vertexBuffer, colorBuffer, normalBuffer, texCoordBuffer;
    ElementBuffer indexBuffer;
};

class EdgeMesh {
public:
    void fromMesh(const Mesh &mesh);
    void createGPUData(Engine *engine);
    std::vector<vec3> vertices;                  // vertex positions
    std::vector<vec3> directions;                // direction to displace vert
    std::vector<vec3> leftNormals, rightNormals; // normals of adj. faces
    std::vector<ivec3> triangles;                // fin triangles
    VertexBuffer vertexBuffer, directionBuffer, leftNormalBuffer, rightNormalBuffer;
    ElementBuffer indexBuffer;
protected:
    typedef std::map<std::pair<int,int>,int> EdgeMap;
    EdgeMap edgeMap;
    void addEdge(const Mesh &mesh, int v0, int v1, vec3 n);
};

inline void Mesh::createGPUData(Engine *engine) {
    vertexBuffer = engine->allocateVertexBuffer(vertices);
    if (!colors.empty())
        colorBuffer = engine->allocateVertexBuffer(colors);
    if (!normals.empty())
        normalBuffer = engine->allocateVertexBuffer(normals);
    if (!texCoords.empty())
        texCoordBuffer = engine->allocateVertexBuffer(texCoords);
    std::vector<int> indices;
    for (int t = 0; t < triangles.size(); t++) {
        indices.push_back(triangles[t][0]);
        indices.push_back(triangles[t][1]);
        indices.push_back(triangles[t][2]);
    }
    indexBuffer = engine->allocateElementBuffer(indices);
}

// Extremely crude OBJ file loader. May not work for general OBJ files.
// Do not use outside of the simple example meshes provided in this course.
inline void Mesh::loadOBJ(const std::string &filename) {
    std::fstream file(filename.c_str(), std::ios::in);
    if (!file) {
        Engine::errorMessage("Failed to load " + filename);
        exit(EXIT_FAILURE);
    }
    while (file) {
        std::string line;
        do
            getline(file, line);
        while (file && (line.length() == 0 || line[0] == '#'));
        std::stringstream linestream(line);
        std::string keyword;
        linestream >> keyword;
        if (keyword == "v") {
            vec3 vertex;
            linestream >> vertex[0] >> vertex[1] >> vertex[2];
            vertices.push_back(vertex);
        } else if (keyword == "vn") {
            vec3 normal;
            linestream >> normal[0] >> normal[1] >> normal[2];
            normals.push_back(normal);
        } else if (keyword == "vt") {
            vec2 texCoord;
            linestream >> texCoord[0] >> texCoord[1];
            texCoords.push_back(texCoord);
        } else if (keyword == "f") {
            std::vector<int> polygon;
            std::string word;
            while (linestream >> word) {
                std::stringstream wstream(word);
                int v;
                wstream >> v;
                polygon.push_back(v-1); // In OBJ files, indices start from 1
            }
            for (int i = 2; i < polygon.size(); i++)
                triangles.push_back(ivec3(polygon[0], polygon[i-1], polygon[i]));
        }
    }
}

inline void EdgeMesh::fromMesh(const Mesh &mesh) {
    for (int t = 0; t < mesh.triangles.size(); t++) {
        ivec3 tri = mesh.triangles[t];
        vec3 a = mesh.vertices[tri[0]],
             b = mesh.vertices[tri[1]],
             c = mesh.vertices[tri[2]];
        vec3 n = glm::normalize(glm::cross(b-a, c-a));
        addEdge(mesh, tri[0], tri[1], n);
        addEdge(mesh, tri[1], tri[2], n);
        addEdge(mesh, tri[2], tri[0], n);
    }
}

inline void EdgeMesh::addEdge(const Mesh &mesh, int v0, int v1, vec3 n) {
    EdgeMap::iterator it = edgeMap.find(std::make_pair(v1, v0));
    // edgeMap contains the index of the first of four consecutive vertices
    // that form the quad fin
    if (it != edgeMap.end()) { // found
        int v = it->second;
        rightNormals[v+0]
            = rightNormals[v+1]
            = rightNormals[v+2]
            = rightNormals[v+3]
            = n;
    } else {
        int v = vertices.size();
        edgeMap[std::make_pair(v0, v1)] = v;
        vertices.push_back(mesh.vertices[v0]);
        vertices.push_back(mesh.vertices[v0]);
        vertices.push_back(mesh.vertices[v1]);
        vertices.push_back(mesh.vertices[v1]);
        directions.push_back(vec3(0,0,0));
        directions.push_back(mesh.normals[v0]);
        directions.push_back(vec3(0,0,0));
        directions.push_back(mesh.normals[v1]);
        leftNormals.push_back(n);
        leftNormals.push_back(n);
        leftNormals.push_back(n);
        leftNormals.push_back(n);
        rightNormals.push_back(-n);
        rightNormals.push_back(-n);
        rightNormals.push_back(-n);
        rightNormals.push_back(-n);
        triangles.push_back(ivec3(v+0,v+2,v+3));
        triangles.push_back(ivec3(v+0,v+3,v+1));
    }
}

inline void EdgeMesh::createGPUData(Engine *engine) {
    vertexBuffer = engine->allocateVertexBuffer(vertices);
    directionBuffer = engine->allocateVertexBuffer(directions);
    leftNormalBuffer = engine->allocateVertexBuffer(leftNormals);
    rightNormalBuffer = engine->allocateVertexBuffer(rightNormals);
    std::vector<int> indices;
    for (int t = 0; t < triangles.size(); t++) {
        indices.push_back(triangles[t][0]);
        indices.push_back(triangles[t][1]);
        indices.push_back(triangles[t][2]);
    }
    indexBuffer = engine->allocateElementBuffer(indices);
}

#endif
