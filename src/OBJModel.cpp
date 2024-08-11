#include "OBJModel.h"
#include <iostream>
#include <fstream>
#include <sstream>


OBJModel::OBJModel()
{
}

OBJModel::~OBJModel()
{
}

void OBJModel::LoadFromFile(const char* fileName)
{
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    std::cerr << "Failed to open OBJ file: " << fileName << std::endl;
    return;
  }

  std::vector<Position> positions;
  std::vector<Normal> normals;
  std::vector<glm::vec2> texCoords; // For texture coordinates

  std::string line;
  std::string currentObject;

  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "o") // Object name
    {
      iss >> currentObject;
    }
    else if (prefix == "v") // Vertex position
    {
      Position position;
      iss >> position.x >> position.y >> position.z;
      positions.push_back(position);
    }
    else if (prefix == "vn") // Vertex normal
    {
      Normal normal;
      iss >> normal.nx >> normal.ny >> normal.nz;
      normals.push_back(normal);
    }
    else if (prefix == "vt") // Texture coordinate
    {
      glm::vec2 texCoord;
      iss >> texCoord.x >> texCoord.y;
      texCoords.push_back(texCoord);
    }
    else if (prefix == "f") // Face
    {
      for (int i = 0; i < 3; i++) // Assuming triangular faces
      {
        unsigned int posIndex, texIndex, normIndex;
        char slash;

        iss >> posIndex >> slash >> texIndex >> slash >> normIndex;

        // Adjust for 1-based indexing in OBJ format
        posIndex--;
        texIndex--;
        normIndex--;

        // Add vertex data to mVertexData
        AddVertexData(posIndex, texIndex, normIndex, positions, texCoords, normals);
      }
    }
  }

  file.close();
}




std::vector<float> OBJModel::GetVertexData()
{
    return mVertexData;
}

int OBJModel::GetVertexCount()
{
    return mVertexData.size() / 9; // 9 is the number of floats in one entry
}

void OBJModel::LoadMaterialFile(const char* fileName)
{
  std::ifstream file(fileName);
  if (file)
  {
    std::string line;
    while (std::getline(file, line))
    {
      char mtlName[100];
      if (StartWith(line, "newmt1"))
      {
        (void)sscanf_s(line.c_str(), "newmt1 %s", mtlName, sizeof(mtlName));
        mMaterialMap[mtlName] = Color();
      }
      if (StartWith(line, "Kd"))
      {
        Color& color = mMaterialMap[mtlName];
        sscanf_s(line.c_str(), "Kd %f %f %f", &color.r, &color.g, &color.b);
      }
    }

  }
  else
  {
    std::cout << " Material file not loaded properly" << std::endl;
  }
}

bool OBJModel::StartWith(std::string& line, const char* text)
{
  size_t textLen = strlen(text);
  if (line.size() < textLen) {
    return false;
  }
  for (size_t i = 0; i < textLen; i++) {
    if (line[i] == text[i]) continue;
    else return false;
  }
    return true;
}

void OBJModel::AddVertexData(int vIdx, int vtIdx, int nIdx, std::vector<Position>& vertices, std::vector<glm::vec2>& texCoords, std::vector<Normal>& normals)
{
  // Check if the vertex index is within bounds
  if (vIdx < 0 || vIdx >= vertices.size())
  {
    std::cerr << "Vertex index out of bounds: " << vIdx << std::endl;
    return;
  }

  // Check if the texture coordinate index is within bounds
  if (vtIdx < 0 || vtIdx >= texCoords.size())
  {
    std::cerr << "Texture coordinate index out of bounds: " << vtIdx << std::endl;
    return;
  }

  // Check if the normal index is within bounds
  if (nIdx < 0 || nIdx >= normals.size())
  {
    std::cerr << "Normal index out of bounds: " << nIdx << std::endl;
    return;
  }

  // Add position
  mVertexData.push_back(vertices[vIdx].x);
  mVertexData.push_back(vertices[vIdx].y);
  mVertexData.push_back(vertices[vIdx].z);

  // Add texture coordinates
  mVertexData.push_back(texCoords[vtIdx].x);
  mVertexData.push_back(texCoords[vtIdx].y);

  // Add normal
  mVertexData.push_back(normals[nIdx].nx);
  mVertexData.push_back(normals[nIdx].ny);
  mVertexData.push_back(normals[nIdx].nz);

  // Optionally, add a color from the material if needed
  // mVertexData.push_back(color.r);
  // mVertexData.push_back(color.g);
  // mVertexData.push_back(color.b);
}



