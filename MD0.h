#pragma once
#include "Mesh.h"

//Own model based on QMDL idea;
constexpr size_t kMD0MagicLettersMax = 4;
constexpr size_t kMD0TextureNameMax = 64;
constexpr size_t kMD0TagsCountMax = 3;
constexpr char kMD0Magic[kMD0MagicLettersMax] = { 'M','D','L','0' };
constexpr int32 kMD0Version = 1;

//MD0;
using float32 = float;
using vec_t = GLfloat;
using vec3_t = vec_t[3];
using vec2_t = vec_t[2];
using int3_t = int32[3];

struct md0_uv
{
	vec2_t uv;
};

struct md0_indexes
{
	int3_t triangle;
};

struct md0_tag
{
	vec3_t tag_position;
	vec3_t tag_rotation;
};

struct md0_frame_vertex
{
	vec3_t position;
	vec3_t normal;
	vec3_t tangent;
};

struct md0_frame
{
	md0_frame_vertex* vertices;
};

struct md0
{
	char ident[kMD0MagicLettersMax]; //Magic MDL0;
	int32 version; //Version of md0;
	int32 triangles_count; //Count of triangles;
	int32 vertices_count; //Count of vertices
	int32 frames_count; //Count of frames for 2D array;
	int32 tags_count; //Count of tags; Should be less than kMD0TagsCountMax;
	char name[kMD0TextureNameMax]; //Texture name;
	char tags_names[kMD0TagsCountMax][kMD0MagicLettersMax]; //Names of tags;
	md0_uv* uvs; //Array of uvs;
	md0_frame* frames; //Array of frames;
	md0_indexes* indexes; //Array of indexes;
	md0_tag* tags; //Array of tags; Count should be equal to frames_count;
};

//Frames count it's a size of frames array;

class MD0
{
private:
	vector<char> m_buffer{}; //Read all bytes in the vector;

	int32 m_triangles_count;
	int32 m_vertices_count;
	int32 m_frames_count;
	int32 m_tags_count;
	char m_texture_name[kMD0TextureNameMax];
	char m_tags_names[kMD0TagsCountMax][kMD0MagicLettersMax];

	vector<md0_uv> m_uvs{};
	vector<md0_indexes> m_indexes{};
	vector<md0_frame> m_frames{};
	vector<md0_tag> m_tags;

	//The mesh only with first frame;
	Mesh* m_mesh;

	//Methods;
	void makeMD0Structure();
	void makeMesh();
public:
	MD0() = default;
	~MD0();
	void loadMD0(const char* p_name);
	const Mesh* getMeshes();
};

