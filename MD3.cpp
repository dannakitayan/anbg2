#include "MD3.h"
#include "Debug.h"
#include "Path.h"
#include <fstream>

MD3::~MD3()
{
	if (m_meshes.size() < 1) return;
	for (auto l_mesh : m_meshes)
	{
		delete(l_mesh);
	}

	m_frames.clear();
	m_tags.clear();
}

void MD3::loadMD3(const char* p_name)
{
	if (!p_name || !p_name[0]) {
		debugLog("[MD3::loadMD3] NULL name.");
		return;
	}

	if (strlen(p_name) >= MAX_QPATH)
	{
		debugLog("[MD3::loadMD3] Model name exceeds MAX_QPATH.");
		return;
	}

	const string l_kFullPath = getAssetPath(Folder::Models, p_name);

	if (l_kFullPath.empty())
	{
		debugLog("[MD3::loadMD3] Failed to load a MD3 file in: ", l_kFullPath.c_str());
		return;
	}

	std::ifstream l_file(l_kFullPath, std::ios::in | std::ios::binary);

	if (!l_file.is_open())
	{
		debugLog("[MD3::loadMD3] Failed to open a MD3 file in:", l_kFullPath.c_str());
		return;
	}

	md3Header_t l_header{};
	l_file.read(reinterpret_cast<char*>(&l_header), sizeof(md3Header_t));

	if (l_header.version != MD3_VERSION) {
		debugLog("[MD3::loadMD3] MD3 file has wrong version; It should be:", std::to_string(MD3_VERSION).c_str());
		return;
	}

	if (l_header.numFrames < 1) {
		debugLog("[MD3::loadMD3] MD3 file no frames");
		return;
	}

	l_file.seekg(l_header.ofsFrames, std::ios::beg);
	m_frames.reserve(l_header.numFrames);
	l_file.read(reinterpret_cast<char*>(m_frames.data()),
		sizeof(md3Frame_t) * l_header.numFrames);

	l_file.seekg(l_header.ofsTags, std::ios::beg);
	m_tags.reserve(l_header.numTags);
	l_file.read(reinterpret_cast<char*>(m_tags.data()),
		sizeof(md3Tag_s) * l_header.numTags);

	std::streamoff l_surface_offset = l_header.ofsSurfaces;

	for (int i = 0; i < l_header.numSurfaces; i++)
	{
		l_file.seekg(l_surface_offset, std::ios::beg);

		md3Surface_t l_surface{};
		l_file.read(reinterpret_cast<char*>(&l_surface), sizeof(md3Surface_t));

		//Names of shaders (textures);
		l_file.seekg(l_surface_offset + l_surface.ofsShaders, std::ios::beg);
		vector<md3Shader_t> l_shaders{};
		l_shaders.reserve(l_surface.numShaders);
		l_file.read(reinterpret_cast<char*>(l_shaders.data()),
			sizeof(md3Shader_t) * l_surface.numShaders);

		//Triagles;
		l_file.seekg(l_surface_offset + l_surface.ofsTriangles, std::ios::beg);
		vector<md3Triangle_t> l_triangles{};
		l_triangles.reserve(l_surface.numTriangles);
		l_file.read(reinterpret_cast<char*>(l_triangles.data()),
			sizeof(md3Triangle_t) * l_surface.numTriangles);

		//UVs;
		l_file.seekg(l_surface_offset + l_surface.ofsSt, std::ios::beg);
		vector<md3St_t> l_uvs{};
		l_uvs.reserve(l_surface.numVerts);
		l_file.read(reinterpret_cast<char*>(l_uvs.data()),
			sizeof(md3St_t) * l_surface.numVerts);

		//Normals;
		l_file.seekg(l_surface_offset + l_surface.ofsXyzNormals, std::ios::beg);
		vector<md3XyzNormal_t> l_normals{};
		l_normals.reserve(l_surface.numVerts);
		l_file.read(reinterpret_cast<char*>(l_normals.data()),
			sizeof(md3XyzNormal_t) * l_surface.numVerts);
	}
}
