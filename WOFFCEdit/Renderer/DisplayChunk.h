#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "../Tool/ChunkObject.h"

//Geometric resolution
//Note: hard coded
#define TERRAIN_RESOLUTION 128

class DisplayChunk
{
public:
	DisplayChunk();
	~DisplayChunk();

	void PopulateChunkData(const ChunkObject* sceneChunk);
	void RenderBatch(std::shared_ptr<DX::DeviceResources>  devResources);
	void InitialiseBatch();													//Initial setup - base coordinates, etc. based on scale
	void LoadHeightMap(std::shared_ptr<DX::DeviceResources>  devResources);
	void SaveHeightMap();													//Saves the heightmap back to file
	void UpdateTerrain();													//Updates the geometry based on the heightmap
	void GenerateHeightmap();												//Creates or alters the heightmap

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>  m_batch;
	std::unique_ptr<DirectX::BasicEffect>       m_terrainEffect;

	ID3D11ShaderResourceView *					m_texture_diffuse;			//Diffuse texture
	Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_terrainInputLayout;

private:
	DirectX::VertexPositionNormalTexture m_terrainGeometry[TERRAIN_RESOLUTION][TERRAIN_RESOLUTION];
	BYTE m_heightMap[TERRAIN_RESOLUTION * TERRAIN_RESOLUTION];
	void CalculateTerrainNormals();

	float	m_terrainHeightScale;
	int		m_terrainSize;					//Size of terrain in metres
	float	m_textureCoordStep;				//Step in texture coordinates between each vertex row / column
	float   m_terrainPositionScalingFactor;	//Factor we multiply the position by to convert it from its native resolution (0-Terrain Resolution) to full scale size in metres dictated by m_terrainSize
	
	std::string m_name;
	int m_chunk_x_size_metres;
	int m_chunk_y_size_metres;
	int m_chunk_base_resolution;

	std::string m_height_map_path;
	std::string m_tex_diffuse_path;
	std::string m_tex_splat_alpha_path;
	std::string m_tex_splat_1_path;
	std::string m_tex_splat_2_path;
	std::string m_tex_splat_3_path;
	std::string m_tex_splat_4_path;

	bool m_render_wireframe;
	bool m_render_normals;

	int m_tex_diffuse_tiling;
	int m_tex_splat_1_tiling;
	int m_tex_splat_2_tiling;
	int m_tex_splat_3_tiling;
	int m_tex_splat_4_tiling;
};