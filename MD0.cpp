#include "MD0.h"
#include "Debug.h"
#include "Path.h"
#include <fstream>
#pragma pack(push, 1)

MD0::~MD0()
{

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
		makeMesh();
	}
}

void MD0::makeMesh()
{
	if (m_buffer.size() <= 0)
	{
		debugLog("[MD0::makeMesh] MD0 file isn't exist, please load it");
	}

	//Read an array;

	//Read magic;
	size_t l_offset = 0;
	char l_ident[kMD0MagicLettersMax]{ 0 };
	std::memcpy(&l_ident, &m_buffer[l_offset], sizeof(char[kMD0MagicLettersMax]));
	if (l_ident[0] != kMD0Magic[0])
	{
		debugLog("[MD0::makeMesh] diffent MD0 file, this", l_ident);
		return;
	}
	debugLog("[MD0::makeMesh] MD0 file ident is", l_ident);

	//Read a model version;
	l_offset += sizeof(char[kMD0MagicLettersMax]);
	int32 l_version = 0;
	std::memcpy(&l_version, &m_buffer[l_offset], sizeof(int32));
	if (l_version != kMD0Version)
	{
		debugLog("[MD0::makeMesh] diffent MD0 file version, this", std::to_string(l_version).c_str());
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMesh] MD0 file version is", std::to_string(l_version).c_str());

	//Read triangles count;
	l_offset += sizeof(int32);
	int32 l_triangles_count = 0;
	std::memcpy(&l_triangles_count, &m_buffer[l_offset], sizeof(int32));
	if (l_triangles_count == 0)
	{
		debugLog("[MD0::makeMesh] no count of triangles in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMesh] MD0 file triangles count", std::to_string(l_triangles_count).c_str());

	//Read vertices count;
	l_offset += sizeof(int32);
	int32 l_vertices_count = 0;
	std::memcpy(&l_vertices_count, &m_buffer[l_offset], sizeof(int32));
	if (l_vertices_count == 0)
	{
		debugLog("[MD0::makeMesh] no count of vertices in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMesh] MD0 file vertices count", std::to_string(l_vertices_count).c_str());

	//Read frames count;
	l_offset += sizeof(int32);
	int32 l_frames_count = 0;
	std::memcpy(&l_frames_count, &m_buffer[l_offset], sizeof(int32));
	if (l_frames_count == 0)
	{
		debugLog("[MD0::makeMesh] no frames in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMesh] MD0 file frames count", std::to_string(l_frames_count).c_str());

	//Read texture name;
	char l_texture_name[kMD0TextureNameMax]{ 0 };
	l_offset += sizeof(int32);
	std::memcpy(&l_texture_name, &m_buffer[l_offset], sizeof(char[kMD0TextureNameMax]));
	if (l_texture_name[0] == 0)
	{
		debugLog("[MD0::makeMesh] no texture in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMesh] MD0 file texture name is", l_texture_name);

	//Read vertices;
	md0_vertex* l_vertices_pointer = new md0_vertex[l_vertices_count];
	l_offset += sizeof(char[kMD0TextureNameMax]);
	std::memcpy(l_vertices_pointer, &m_buffer[l_offset], sizeof(md0_vertex) * l_vertices_count);
	if (l_vertices_pointer == nullptr)
	{
		debugLog("[MD0::makeMesh] no actual vertices in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMesh] MD0 file vertices are loader");

	//Read triangles;
	md0_indexes* l_indexes_pointer = new md0_indexes[l_triangles_count];
	l_offset += sizeof(md0_vertex) * l_vertices_count;
	std::memcpy(l_indexes_pointer, &m_buffer[l_offset], sizeof(md0_indexes) * (l_triangles_count));
	if (l_indexes_pointer == nullptr)
	{
		debugLog("[MD0::makeMesh] no actual indexes in MD0 file");
		m_buffer.clear();
		return;
	}
	debugLog("[MD0::makeMesh] MD0 file indexes are loader");

	//Prepare vertices and indexes;
	m_vertices.resize(l_vertices_count * 11);
	m_indexes.resize(l_triangles_count * 3);

	std::memcpy(
		m_vertices.data(),                  //where to
		l_vertices_pointer,                 //from
		sizeof(float) * 11 * l_vertices_count //how much bytes
	);

	std::memcpy(
		m_indexes.data(),
		l_indexes_pointer,
		sizeof(int) * 3 * l_triangles_count
	);

	m_meshes_count = 1;
	m_buffer.clear();

	if (m_meshes_count <= 0) return;
	for (int i = 0; i < m_meshes_count; i++)
	{
		Mesh* l_p_mesh = new Mesh(m_vertices, m_indexes, m_texture_name.c_str());
		m_meshes.push_back(l_p_mesh);
		debugLog("[MD0::makeMesh] Mesh is pushed, number", (std::to_string(i)).c_str());
		debugLog("[MD0::makeMesh] Mesh has vertices count", (std::to_string(m_vertices.size() / 11)).c_str());
		debugLog("[MD0::makeMesh] Mesh has indexes count", (std::to_string(m_indexes.size() / 3)).c_str());
	}
}

const vector<Mesh*>& MD0::getMeshes()
{
	return m_meshes;
}