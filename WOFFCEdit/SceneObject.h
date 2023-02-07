#pragma once
#include <string>
using std::string;
//This object should accurately and totally reflect the information stored in the object table

class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	//IDs
	int ID;
	int chunk_ID;

	//File Paths
	string model_path;
	string tex_diffuse_path;

	//Position, Rotation, & Scale
	float posX, posY, posZ;
	float rotX, rotY, rotZ;
	float scaX, scaY, scaZ;

	//Object Functionality Booleans
	bool render, collision;
	bool collectable, destructable;
	string collision_mesh;

	//Health
	int health_amount;

	//Engine-Specific Functionality
	bool editor_render, editor_texture_vis;
	bool editor_normals_vis, editor_collision_vis, editor_pivot_vis;
	bool snapToGround;
	float pivotX, pivotY, pivotZ;

	//AI
	bool AINode;

	//Audio
	string audio_path;
	float volume;
	float pitch;
	float pan;
	bool one_shot;
	bool play_on_init;
	bool play_in_editor;
	int min_dist;
	int max_dist;

	//Camera Functionality
	bool camera;
	bool path_node;
	bool path_node_start;
	bool path_node_end;

	//Parenting
	int parent_id;

	//Wireframe Mode
	bool editor_wireframe;

	//Object Name
	string name;

	//Lights
	int light_type;
	float light_diffuse_r, light_diffuse_g, light_diffuse_b;
	float light_specular_r, light_specular_g, light_specular_b;
	float light_spot_cutoff;
	float light_constant;
	float light_linear;
	float light_quadratic;
};