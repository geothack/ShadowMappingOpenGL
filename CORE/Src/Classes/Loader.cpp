#include "Loader.h"


static bool CompareIndexPtr(const Index* a, const Index* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string& s, char delim);

ModelArray::ModelArray(const std::string& fileName)
{
    HasUVs = false;
    HasNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if (file.is_open())
    {
        while (file.good())
        {
            getline(file, line);

            unsigned int lineLength = line.length();

            if (lineLength < 2)
                continue;

            const char* lineCStr = line.c_str();

            switch (lineCStr[0])
            {
            case 'v':
                if (lineCStr[1] == 't')
                    this->Uvs.push_back(ParseOBJVec2(line));
                else if (lineCStr[1] == 'n')
                    this->Normals.push_back(ParseOBJVec3(line));
                else if (lineCStr[1] == ' ' || lineCStr[1] == '\t')
                    this->Vertices.push_back(ParseOBJVec3(line));
                break;
            case 'f':
                CreateOBJFace(line);
                break;
            default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

void ModelIndex::CalcNormals()
{
    for (unsigned int i = 0; i < Indices.size(); i += 3)
    {
        int i0 = Indices[i];
        int i1 = Indices[i + 1];
        int i2 = Indices[i + 2];

        glm::vec3 v1 = Positions[i1] - Positions[i0];
        glm::vec3 v2 = Positions[i2] - Positions[i0];

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        Normals[i0] += normal;
        Normals[i1] += normal;
        Normals[i2] += normal;
    }

    for (unsigned int i = 0; i < Positions.size(); i++)
        Normals[i] = glm::normalize(Normals[i]);
}

ModelIndex ModelArray::ToModelIndex()
{
    ModelIndex result;
    ModelIndex normalModel;

    unsigned int numIndices = Indices.size();

    std::vector<Index*> indexLookup;

    for (unsigned int i = 0; i < numIndices; i++)
        indexLookup.push_back(&Indices[i]);

    std::sort(indexLookup.begin(), indexLookup.end(), CompareIndexPtr);

    std::map<Index, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;

    for (unsigned int i = 0; i < numIndices; i++)
    {
        Index* currentIndex = &Indices[i];

        glm::vec3 currentPosition = Vertices[currentIndex->VertexIndex];
        glm::vec2 currentTexCoord;
        glm::vec3 currentNormal;

        if (HasUVs)
            currentTexCoord = Uvs[currentIndex->UvIndex];
        else
            currentTexCoord = glm::vec2(0, 0);

        if (HasNormals)
            currentNormal = Normals[currentIndex->NormalIndex];
        else
            currentNormal = glm::vec3(0, 0, 0);

        unsigned int normalModelIndex;
        unsigned int resultModelIndex;

        //Create model to properly generate normals on
        std::map<Index, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if (it == normalModelIndexMap.end())
        {
            normalModelIndex = normalModel.Positions.size();

            normalModelIndexMap.insert(std::pair<Index, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.Positions.push_back(currentPosition);
            normalModel.TexCoords.push_back(currentTexCoord);
            normalModel.Normals.push_back(currentNormal);
        }
        else
            normalModelIndex = it->second;

        //Create model which properly separates texture coordinates
        unsigned int previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, result);

        if (previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = result.Positions.size();

            result.Positions.push_back(currentPosition);
            result.TexCoords.push_back(currentTexCoord);
            result.Normals.push_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;

        normalModel.Indices.push_back(normalModelIndex);
        result.Indices.push_back(resultModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }

    if (!HasNormals)
    {
        normalModel.CalcNormals();

        for (unsigned int i = 0; i < result.Positions.size(); i++)
            result.Normals[i] = normalModel.Normals[indexMap[i]];
    }

    return result;
};

unsigned int ModelArray::FindLastVertexIndex(const std::vector<Index*>& indexLookup, const Index* currentIndex, const ModelIndex& result)
{
    unsigned int start = 0;
    unsigned int end = indexLookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;

    while (current != previous)
    {
        Index* testIndex = indexLookup[current];

        if (testIndex->VertexIndex == currentIndex->VertexIndex)
        {
            unsigned int countStart = current;

            for (unsigned int i = 0; i < current; i++)
            {
                Index* possibleIndex = indexLookup[current - i];

                if (possibleIndex == currentIndex)
                    continue;

                if (possibleIndex->VertexIndex != currentIndex->VertexIndex)
                    break;

                countStart--;
            }

            for (unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
            {
                Index* possibleIndex = indexLookup[current + i];

                if (possibleIndex == currentIndex)
                    continue;

                if (possibleIndex->VertexIndex != currentIndex->VertexIndex)
                    break;
                else if ((!HasUVs || possibleIndex->UvIndex == currentIndex->UvIndex)
                    && (!HasNormals || possibleIndex->NormalIndex == currentIndex->NormalIndex))
                {
                    glm::vec3 currentPosition = Vertices[currentIndex->VertexIndex];
                    glm::vec2 currentTexCoord;
                    glm::vec3 currentNormal;

                    if (HasUVs)
                        currentTexCoord = Uvs[currentIndex->UvIndex];
                    else
                        currentTexCoord = glm::vec2(0, 0);

                    if (HasNormals)
                        currentNormal = Normals[currentIndex->NormalIndex];
                    else
                        currentNormal = glm::vec3(0, 0, 0);

                    for (unsigned int j = 0; j < result.Positions.size(); j++)
                    {
                        if (currentPosition == result.Positions[j]
                            && ((!HasUVs || currentTexCoord == result.TexCoords[j])
                                && (!HasNormals || currentNormal == result.Normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }

            return -1;
        }
        else
        {
            if (testIndex->VertexIndex < currentIndex->VertexIndex)
                start = current;
            else
                end = current;
        }

        previous = current;
        current = (end - start) / 2 + start;
    }

    return -1;
}

void ModelArray::CreateOBJFace(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->Indices.push_back(ParseIndex(tokens[1], &this->HasUVs, &this->HasNormals));
    this->Indices.push_back(ParseIndex(tokens[2], &this->HasUVs, &this->HasNormals));
    this->Indices.push_back(ParseIndex(tokens[3], &this->HasUVs, &this->HasNormals));

    if ((int)tokens.size() > 4)
    {
        this->Indices.push_back(ParseIndex(tokens[1], &this->HasUVs, &this->HasNormals));
        this->Indices.push_back(ParseIndex(tokens[3], &this->HasUVs, &this->HasNormals));
        this->Indices.push_back(ParseIndex(tokens[4], &this->HasUVs, &this->HasNormals));
    }
}

Index ModelArray::ParseIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    unsigned int tokenLength = token.length();
    const char* tokenString = token.c_str();

    unsigned int vertIndexStart = 0;
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

    Index result;
    result.VertexIndex = ParseIndexValue(token, vertIndexStart, vertIndexEnd);
    result.UvIndex = 0;
    result.NormalIndex = 0;

    if (vertIndexEnd >= tokenLength)
        return result;

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

    result.UvIndex = ParseIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;

    if (vertIndexEnd >= tokenLength)
        return result;

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

    result.NormalIndex = ParseIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;

    return result;
}

glm::vec3 ModelArray::ParseOBJVec3(const std::string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();

    unsigned int vertIndexStart = 2;

    while (vertIndexStart < tokenLength)
    {
        if (tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }

    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    return glm::vec3(x, y, z);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 ModelArray::ParseOBJVec2(const std::string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();

    unsigned int vertIndexStart = 3;

    while (vertIndexStart < tokenLength)
    {
        if (tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }

    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    return glm::vec2(x, y);
}

static bool CompareIndexPtr(const Index* a, const Index* b)
{
    return a->VertexIndex < b->VertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;
    while (result < length)
    {
        result++;
        if (str[result] == token)
            break;
    }

    return result;
}

static inline unsigned int ParseIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string& s, char delim)
{
    std::vector<std::string> elems;

    const char* cstr = s.c_str();
    unsigned int strLength = s.length();
    unsigned int start = 0;
    unsigned int end = 0;

    while (end <= strLength)
    {
        while (end <= strLength)
        {
            if (cstr[end] == delim)
                break;
            end++;
        }

        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }

    return elems;
}
