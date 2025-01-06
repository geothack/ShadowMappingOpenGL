#pragma once
#include <glm.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

struct Index
{
    unsigned int VertexIndex;
    unsigned int UvIndex;
    unsigned int NormalIndex;

    bool operator<(const Index& r) const { return VertexIndex < r.VertexIndex; }
};

class ModelIndex
{
public:
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
    std::vector<unsigned int> Indices;

    void CalcNormals();
};

class ModelArray
{
public:
    std::vector<Index> Indices;
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec2> Uvs;
    std::vector<glm::vec3> Normals;
    bool HasUVs;
    bool HasNormals;

    ModelArray(const std::string& fileName);

    ModelIndex ToModelIndex();
private:
    unsigned int FindLastVertexIndex(const std::vector<Index*>& indexLookup, const Index* currentIndex, const ModelIndex& result);
    void CreateOBJFace(const std::string& line);

    glm::vec2 ParseOBJVec2(const std::string& line);
    glm::vec3 ParseOBJVec3(const std::string& line);
    Index ParseIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};




