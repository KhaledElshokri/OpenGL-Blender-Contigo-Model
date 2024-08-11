#pragma once
#include <vector>
#include <map>
#include <string>
#include "glm/glm.hpp"

class OBJModel
{
public:
	OBJModel();
	~OBJModel();

	void LoadFromFile(const char* fileName);
	std::vector<float> GetVertexData();
	int GetVertexCount();

private:
	struct Position { float x, y, z; };
	struct Color { float r, g, b; };
	struct Normal { float nx, ny, nz; };

	void LoadMaterialFile(const char* fileName);
	bool StartWith(std::string& line, const char* text);
	void AddVertexData(int vIdx, int vtIdx, int nIdx, std::vector<Position>& vertices, std::vector<glm::vec2>& texCoords, std::vector<Normal>& normals);

	std::map<std::string, Color> mMaterialMap;
	std::vector<float> mVertexData;

};