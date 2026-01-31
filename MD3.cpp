#include "MD3.h"
#include "Debug.h"
#include "Path.h"
#include <fstream>
#pragma pack(push, 1)

void MD3::makeMesh()
{
	if (mesh_count <= 0) return;
	for (int i = 0; i < mesh_count; i++)
	{
		Mesh* l_p_mesh = new Mesh(m_vertices, m_indexes, "test.png");
		m_meshes.push_back(l_p_mesh);
		debugLog("[MD3::makeMesh] Mesh is pushed, number", (std::to_string(i)).c_str());
		debugLog("[MD3::makeMesh] Mesh has vertices count", (std::to_string(m_vertices.size() / 2 / 11)).c_str());
		debugLog("[MD3::makeMesh] Mesh has indexes count", (std::to_string(m_indexes.size())).c_str());
	}
}

MD3::~MD3()
{
	if (m_meshes.size() < 1) return;
	for (auto l_mesh : m_meshes)
	{
		delete(l_mesh);
	}

	m_frames.clear();
	m_tags.clear();
	m_vertices.clear();
	m_indexes.clear();
	m_texture_names.clear();
	mesh_count = 0;
}

void MD3::loadMD3(const char* p_name)
{
	if (!p_name || !p_name[0]) {
		debugLog("[MD3::loadMD3] Nothing to load");
		return;
	}

	if (strlen(p_name) >= MAX_QPATH)
	{
		debugLog("[MD3::loadMD3] Model name exceeds MAX_QPATH");
		return;
	}

	const string l_kFullPath = getAssetPath(Folder::Models, p_name);

	if (l_kFullPath.empty())
	{
		debugLog("[MD3::loadMD3] Failed to load a MD3 file in: ", l_kFullPath.c_str());
		return;
	}

	std::ifstream l_file(l_kFullPath, std::ios::in | std::ios::binary | std::ios::ate);

	if (!l_file.is_open())
	{
		debugLog("[MD3::loadMD3] Failed to open a MD3 file in:", l_kFullPath.c_str());
		return;
	}

	std::streampos l_position = l_file.tellg();
	size_t l_length = static_cast<size_t>(l_position);

	if (l_length <= 0)
	{
		debugLog("[MD3::loadMD3] MD3 file is corrupted");
		return;
	}

	std::vector<byte> l_buffer(l_length);
	l_file.seekg(0, std::ios::beg);
	l_file.read(reinterpret_cast<char*>(l_buffer.data()), l_length);

	l_file.close();

	size_t l_offset = 0;
	md3Header_t l_header{};

	std::memcpy(&l_header, l_buffer.data() + l_offset, sizeof(md3Header_t));

	if (l_header.version != MD3_VERSION) {
		debugLog("[MD3::loadMD3] MD3 file has wrong version; It should be:", std::to_string(MD3_VERSION).c_str());
		return;
	}

	if (l_header.numFrames < 1) {
		debugLog("[MD3::loadMD3] MD3 file no frames");
		return;
	}

	//Frames;
	m_frames.resize(l_header.numFrames);
	size_t l_frame_offset = static_cast<size_t>(l_header.ofsFrames);
	std::memcpy(m_frames.data(), l_buffer.data() + l_frame_offset, l_header.numFrames * sizeof(md3Frame_t));

	//Tags;
	m_tags.resize(l_header.numTags * l_header.numFrames);
	size_t l_tags_offset = static_cast<size_t>(l_header.ofsTags);
	std::memcpy(m_tags.data(), l_buffer.data() + l_tags_offset, l_header.numTags * sizeof(md3Tag_s));

	//Surfaces;
	size_t l_surface_offset = static_cast<size_t>(l_header.ofsSurfaces);

	//Vertices;
	for (int i = 0; i < l_header.numSurfaces; i++)
	{
		md3Surface_t l_surface{};
		std::memcpy(&l_surface, l_buffer.data() + l_surface_offset, sizeof(md3Surface_t));

		vector<md3St_t> l_uv_vector{};
		l_uv_vector.resize(l_surface.numVerts);

		vector<md3XyzNormal_t> l_vertices_vector{};
		l_vertices_vector.resize(l_surface.numVerts * l_surface.numFrames);

		vector<md3Triangle_t> l_triagles_vector{};
		l_triagles_vector.resize(l_surface.numTriangles);

		vector<md3Shader_t> l_shaders_vector{};
		l_shaders_vector.resize(l_surface.numShaders);

		size_t l_uv_offset = static_cast<size_t>(l_surface.ofsSt);
		l_uv_offset += l_surface_offset;

		size_t l_triangles_offset = static_cast<size_t>(l_surface.ofsTriangles);
		l_triangles_offset += l_surface_offset;

		size_t l_vertices_offset = static_cast<size_t>(l_surface.ofsXyzNormals);
		l_vertices_offset += l_surface_offset;

		size_t l_shaders_offset = static_cast<size_t>(l_surface.ofsShaders);
		l_shaders_offset += l_surface_offset;

		std::memcpy(l_uv_vector.data(), l_buffer.data() + l_uv_offset, l_surface.numVerts * sizeof(md3St_t));
		std::memcpy(l_vertices_vector.data(), l_buffer.data() + l_vertices_offset, (l_surface.numVerts * l_surface.numFrames) * sizeof(md3XyzNormal_t));
		std::memcpy(l_triagles_vector.data(), l_buffer.data() + l_triangles_offset, l_surface.numTriangles * sizeof(md3Triangle_t));
		std::memcpy(l_shaders_vector.data(), l_buffer.data() + l_shaders_offset, l_surface.numShaders * sizeof(md3Shader_t));

		//Indexes;
		for (const auto& l_triangle : l_triagles_vector) {
			m_indexes.push_back(l_triangle.indexes[0]);
			m_indexes.push_back(l_triangle.indexes[1]);
			m_indexes.push_back(l_triangle.indexes[2]);
		}

		//Vertices;
		for (int j = 0; j < l_vertices_vector.size(); j++)
		{
			GLfloat l_vertices[kVerticesCount]{};
			//aPos;
			l_vertices[0] = l_vertices_vector[j].xyz[0] *MD3_XYZ_SCALE;
			l_vertices[1] = l_vertices_vector[j].xyz[1] *MD3_XYZ_SCALE;
			l_vertices[2] = l_vertices_vector[j].xyz[2] *MD3_XYZ_SCALE;
			//aColor;
			l_vertices[3] = 1.0f;
			l_vertices[4] = 1.0f;
			l_vertices[5] = 1.0f;
			//aUV;
			l_vertices[6] = l_uv_vector[j % l_surface.numVerts].st[0];
			l_vertices[7] = l_uv_vector[j % l_surface.numVerts].st[1];
			//aNormal;
			float lng = (l_vertices_vector[j].normal & 0xFF) * 2 * Q_PI / 255.0f; // decode vertex normals
			float lat = ((l_vertices_vector[j].normal >> 8) & 0xFF) * 2 * Q_PI / 255.0f;

			l_vertices[8] = cosf(lat) * sinf(lng);
			l_vertices[9] = sinf(lat) * sinf(lng);
			l_vertices[10] = cosf(lng);
			m_vertices.insert(m_vertices.end(), std::begin(l_vertices), std::end(l_vertices));
		}

		//Textures names;
		for (const auto& l_shader : l_shaders_vector) {
			auto p_shader_name = (char*)l_shader.name;
			string l_name = string(p_shader_name);
			m_texture_names.push_back(l_name);
		}

		l_surface_offset += l_surface.ofsEnd;
	}

	mesh_count = l_header.numSurfaces;
	debugLog("[MD3::loadMD3] MD3 file is loaded", p_name);

	makeMesh();
	debugLog("[MD3::loadMD3] Mesh is created", p_name);
}

const vector<Mesh*>& MD3::getMeshes()
{
	return m_meshes;
}
