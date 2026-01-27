#pragma once
#include "BasicTypes.h"

//byte 8 - bit unsigned octet(character).
//short16 little-endian signed 16-bit integer.
//int32 little-endian signed 32-bit integer.
//float32 IEEE-754 32-bit floating-point.
//vec3_t triplet of F32 in sequence 
// (read 4 octets, make float, read 4, make float, read 4, make float), 
// describing a 3-space vector.

constexpr int32 MAX_QPATH = 64;

constexpr auto MD3_IDENT = (('3'<<24)+('P'<<16)+('D'<<8)+'I');
constexpr int32 MD3_VERSION = 15;

// limits
constexpr int32 MD3_MAX_LODS = 3;
constexpr int32	MD3_MAX_TRIANGLES = 8192;	// per surface
constexpr int32 MD3_MAX_VERTS = 4096;		// per surface
constexpr int32 MD3_MAX_SHADERS	= 256;		// per surface
constexpr int32 MD3_MAX_FRAMES = 1024;		// per model
constexpr int32 MD3_MAX_SURFACES = 32;		// per model
constexpr int32 MD3_MAX_TAGS = 16;			// per frame

// vertex scales
constexpr float	MD3_XYZ_SCALE = (1.0/64);

typedef struct md3Frame_s {
	vec3_t		bounds[2];
	vec3_t		localOrigin;
	float32		radius;
	byte		name[16];
} md3Frame_t;

typedef struct md3Tag_s {
	byte		name[MAX_QPATH];	// tag name
	vec3_t		origin;
	vec3_t		axis[3];
} md3Tag_t;

typedef struct {
	int32		ident;				// 

	byte	name[MAX_QPATH];	// polyset name

	int32		flags;
	int32		numFrames;			// all surfaces in a model should have the same

	int32		numShaders;			// all surfaces in a model should have the same
	int32		numVerts;

	int32		numTriangles;
	int32		ofsTriangles;

	int32		ofsShaders;			// offset from start of md3Surface_t
	int32		ofsSt;				// texture coords are common for all frames
	int32		ofsXyzNormals;		// numVerts * numFrames

	int32		ofsEnd;				// next surface
} md3Surface_t;

typedef struct {
	byte			name[MAX_QPATH];
	int32				shaderIndex;	// for in-game use
} md3Shader_t;

typedef struct {
	int32			indexes[3];
} md3Triangle_t;

typedef struct {
	float32		st[2];
} md3St_t;

typedef struct {
	short16		xyz[3];
	short16		normal;
} md3XyzNormal_t;

typedef struct {
	int32			ident;
	int32			version;

	byte		name[MAX_QPATH];	// model name

	int32			flags;

	int32			numFrames;
	int32			numTags;
	int32			numSurfaces;

	int32			numSkins;

	int32			ofsFrames;			// offset for first frame
	int32			ofsTags;			// numFrames * numTags
	int32			ofsSurfaces;		// first surface, others follow

	int32			ofsEnd;				// end of file
} md3Header_t;

class MD3
{
private:
	md3Header_t m_header;
	md3St_t m_uv;
	vector<md3Triangle_t> m_triangles{};
	vector<md3Surface_t> m_surfaces{};
	vector<md3XyzNormal_t> m_normals{};
	vector<md3Shader_t> m_shader{};
	vector<md3Tag_t> m_tag{};
	vector<md3Frame_t> m_frames{};
public:
	MD3();
	~MD3();
	void loadMD3(const char* p_name);
};