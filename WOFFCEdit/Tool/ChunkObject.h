#pragma once

#include <string>

class ChunkObject
{
public:
	ChunkObject();
	~ChunkObject();

	//Identifiers
	int ID;
	std::string name;

	//Chunk Size
	int chunk_x_size_metres;
	int chunk_y_size_metres;

	//Texturing
	int chunk_base_resolution;
	std::string heightmap_path;
	std::string tex_diffuse_path;
	std::string tex_splat_alpha_path;
	std::string tex_splat_1_path;
	std::string tex_splat_2_path;
	std::string tex_splat_3_path;
	std::string tex_splat_4_path;

	//Engine Rendering
	bool render_wireframe;
	bool render_normals;

	//Texture Tiling
	int tex_diffuse_tiling;
	int tex_splat_1_tiling;
	int tex_splat_2_tiling;
	int tex_splat_3_tiling;
	int tex_splat_4_tiling;
};