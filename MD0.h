#pragma once
#include "Mesh.h"

//Own model based on QMDL idea;
constexpr byte kMD0TextureNameMax = 64;
constexpr auto kMD0 = (('M' << 24) + ('D' << 16) + ('L' << 8) + '0');
constexpr int32 kMD0Version = 1;

struct MD0Tag
{
	vec3_t tag_position;
	char tag_name[kMD0TextureNameMax]; //Tag name;
};

struct MD0Begin
{
	int32 ident; //Magic number; MDL0
	int32 version; //Version of md0;
	int32 triangles_count; //Count of triangles;
	int32 vertices_count; //Count of vertices
	int32 frames_count; //Count of frames for 2D array;
	char name[kMD0TextureNameMax]; //Texture name;
	md0_vertex* vertices; //Array of vertices;
	md0_indexes* indexes; //Array of indexes;
	vec3_t* frames; //Array of vectors to store animation data in a image;
	MD0Tag* tags; //Array of tags; Count should be equal to frames_count;
};

//Frames count it's a size of frames array;

class MD0
{
private:
	vector<Mesh*> m_meshes{};
public:
	MD0() = default;
	~MD0();
	void loadMD0(const char* p_name);
};

