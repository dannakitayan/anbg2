#include "MD0.h"
#include "Debug.h"
#include "Path.h"
#include <fstream>
#pragma pack(push, 1)

MD0::~MD0()
{
	for (auto& frame : m_frames)
	{
		delete[] frame.vertices;
		frame.vertices = nullptr;
	}

	delete m_mesh;
	m_mesh = nullptr;
}

void MD0::loadMD0(const char* p_name)
{
	if (!p_name || !p_name[0]) {
		debugLog("[MD0::loadMD0] Nothing to load");
		return;
	}

	if (strlen(p_name) >= kMD0TextureNameMax)
	{
		debugLog("[MD0::loadMD0] Model name exceeds kMD0TextureNameMax");
		return;
	}

	const string l_kFullPath = getAssetPath(Folder::Models, p_name);

	if (l_kFullPath.empty())
	{
		debugLog("[MD0::loadMD0] Failed to load a MD0 file in: ", l_kFullPath.c_str());
		return;
	}

	std::ifstream l_file(l_kFullPath, std::ios::in | std::ios::binary | std::ios::ate);

	if (!l_file.is_open())
	{
		debugLog("[MD0::loadMD0] Failed to open a MD0 file in:", l_kFullPath.c_str());
		return;
	}

	//Get a file length;
	std::streampos l_position = l_file.tellg();
	size_t l_length = static_cast<size_t>(l_position);
	//Set the cursor to begin;
	l_file.seekg(0, std::ios::beg);

	if (l_length <= 0)
	{
		debugLog("[MD0::loadMD0] MD0 file is corrupted");
		return;
	}

	//Read all data into the temporary buffer;
	m_buffer.resize(l_length);
	l_file.read(m_buffer.data(), l_length);

	//Close the stream;
	l_file.close();

	if (m_buffer.size() <= 0)
	{
		debugLog("[MD0::loadMD0] MD0 file is empty, please load it again");
	}
	else
	{
		makeMD0Structure();
	}
}

void MD0::makeMD0Structure()
{
	if (m_buffer.size() <= 0)
	{
		debugLog("[MD0::makeMD0Structure] MD0 file isn't exist, please load it");
	}

	//Read magic;
	size_t l_offset = 0;
	char l_ident[kMD0MagicLettersMax]{ 0 };
	std::memcpy(&l_ident, &m_buffer[l_offset], sizeof(char[kMD0MagicLettersMax]));
	if (std::memcmp(l_ident, kMD0Magic, kMD0MagicLettersMax) != 0)
	{
		debugLog("[MD0::makeMD0Structure] diffent MD0 file, this", l_ident);
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file ident is", l_ident);

	//Read a model version;
	l_offset += sizeof(char[kMD0MagicLettersMax]);
	int32 l_version = 0;
	std::memcpy(&l_version, &m_buffer[l_offset], sizeof(int32));
	if (l_version != kMD0Version)
	{
		debugLog("[MD0::makeMD0Structure] diffent MD0 file version, this", std::to_string(l_version).c_str());
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file version is", std::to_string(l_version).c_str());

	//Read triangles count;
	l_offset += sizeof(int32);
	int32 l_triangles_count = 0;
	std::memcpy(&l_triangles_count, &m_buffer[l_offset], sizeof(int32));
	if (l_triangles_count == 0)
	{
		debugLog("[MD0::makeMD0Structure] no count of triangles in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file triangles count", std::to_string(l_triangles_count).c_str());

	//Read vertices count;
	l_offset += sizeof(int32);
	int32 l_vertices_count = 0;
	std::memcpy(&l_vertices_count, &m_buffer[l_offset], sizeof(int32));
	if (l_vertices_count == 0)
	{
		debugLog("[MD0::makeMD0Structure] no count of vertices in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file vertices count", std::to_string(l_vertices_count).c_str());

	//Read frames count;
	l_offset += sizeof(int32);
	int32 l_frames_count = 0;
	std::memcpy(&l_frames_count, &m_buffer[l_offset], sizeof(int32));
	if (l_frames_count == 0)
	{
		debugLog("[MD0::makeMD0Structure] no frames in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file frames count", std::to_string(l_frames_count).c_str());

	//Read tags count;
	l_offset += sizeof(int32);
	int32 l_tags_count = 0;
	std::memcpy(&l_tags_count, &m_buffer[l_offset], sizeof(int32));
	if (l_tags_count > kMD0TagsCountMax )
	{
		debugLog("[MD0::makeMD0Structure] tags in MD0 file more than kMD0TagsCountMax");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file tags count", std::to_string(l_tags_count).c_str());

	//Read texture name;
	l_offset += sizeof(int32);
	std::memcpy(&m_texture_name, &m_buffer[l_offset], sizeof(char[kMD0TextureNameMax]));
	if (m_texture_name[0] == 0)
	{
		debugLog("[MD0::makeMD0Structure] no texture in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file texture name is", m_texture_name);

	//Read tags names;
	l_offset += sizeof(char[kMD0TextureNameMax]);
	std::memcpy(&m_tags_names, &m_buffer[l_offset], sizeof(char[kMD0TagsCountMax][kMD0MagicLettersMax]));
	debugLog("[MD0::makeMD0Structure] MD0 file all tags are loaded");

	//Read uvs;
	l_offset += sizeof(char[kMD0TagsCountMax][kMD0MagicLettersMax]);

	m_uvs.resize(l_vertices_count);
	std::memcpy(m_uvs.data(), &m_buffer[l_offset], sizeof(md0_uv) * l_vertices_count);
	if (m_uvs.empty())
	{
		debugLog("[MD0::makeMD0Structure] no uv in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file uv is loader");

	//Read frames;
	l_offset += sizeof(md0_uv) * l_vertices_count;
	
	m_frames.resize(l_frames_count);
	for (int32 i = 0; i < l_frames_count; i++)
	{
		m_frames[i].vertices = new md0_frame_vertex[l_vertices_count];
		std::memcpy(m_frames[i].vertices, &m_buffer[l_offset], sizeof(md0_frame_vertex) * l_vertices_count);
		l_offset += sizeof(md0_frame_vertex) * l_vertices_count;
	}

	if (m_frames.empty())
	{
		debugLog("[MD0::makeMD0Structure] no frames in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file frames are loader");

	//Read triangles;
	m_indexes.resize(l_triangles_count);
	std::memcpy(m_indexes.data(), &m_buffer[l_offset], sizeof(md0_indexes)* l_triangles_count);
	if (m_indexes.empty())
	{
		debugLog("[MD0::makeMD0Structure] no actual indexes in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMD0Structure] MD0 file indexes are loader");

	//Read and store frames;
	if (l_tags_count > 0)
	{
		l_offset += sizeof(md0_indexes) * l_triangles_count;
		m_tags.resize(l_tags_count);
		std::memcpy(m_tags.data(), &m_buffer[l_offset], sizeof(md0_tag) * l_tags_count);
		if (m_tags.empty())
		{
			debugLog("[MD0::makeMD0Structure] no actual tags in MD0 file");
			m_buffer.clear();
			return;
		}
		debugLog("[MD0::makeMD0Structure] MD0 file tags are loader");
	}

	m_triangles_count = l_triangles_count;
	m_vertices_count = l_vertices_count;
	m_frames_count = l_frames_count;
	m_tags_count = l_tags_count;

	m_buffer.clear();
	makeMesh();
}

void MD0::makeMesh()
{
	const size_t l_indexes_additional_value = 3;
	const size_t l_vertices_additional_value = 14;
	const size_t l_total_vertices = m_vertices_count * l_vertices_additional_value;
	const size_t l_total_triangles = m_triangles_count * l_indexes_additional_value;
	vector<GLfloat> l_vertices;
	vector<GLuint> l_indexes;
	l_vertices.reserve(l_total_vertices);
	l_indexes.reserve(l_total_triangles);

	md0_frame l_first_frame = m_frames[0];

	//aPos[3], aColors[3], aUV[2], aNormal[3], aTangent[3]; = 14;
	for (size_t i = 0; i < m_vertices_count; ++i)
	{
		//aPos;
		l_vertices.push_back(l_first_frame.vertices[i].position[0]);
		l_vertices.push_back(l_first_frame.vertices[i].position[1]);
		l_vertices.push_back(l_first_frame.vertices[i].position[2]);

		//aColors;
		l_vertices.push_back(0.0f);
		l_vertices.push_back(0.0f);
		l_vertices.push_back(0.0f);

		//aUV;
		l_vertices.push_back(m_uvs[i].uv[0]);
		l_vertices.push_back(m_uvs[i].uv[1]);

		//aNormal;
		l_vertices.push_back(l_first_frame.vertices[i].normal[0]);
		l_vertices.push_back(l_first_frame.vertices[i].normal[1]);
		l_vertices.push_back(l_first_frame.vertices[i].normal[2]);

		//aTangent;
		l_vertices.push_back(l_first_frame.vertices[i].tangent[0]);
		l_vertices.push_back(l_first_frame.vertices[i].tangent[1]);
		l_vertices.push_back(l_first_frame.vertices[i].tangent[2]);
	}

	for ( auto &element : m_indexes)
	{
		l_indexes.push_back(element.triangle[0]);
		l_indexes.push_back(element.triangle[1]);
		l_indexes.push_back(element.triangle[2]);
	}

	m_mesh = new Mesh(l_vertices, l_indexes, m_texture_name);
	debugLog("[MD0::makeMesh] Mesh are generated");
}

const Mesh* MD0::getMeshes()
{
	return m_mesh;
}

#pragma pack(pop)