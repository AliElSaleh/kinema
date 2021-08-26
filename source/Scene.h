#pragma once

#include "VoxelMesh.h"

class Scene
{
public:
	std::vector<VoxelMesh> VoxelMeshes;

	void Render(Device* device, Shader* shader)
	{
		for (VoxelMesh& mesh : VoxelMeshes)
		{
			mesh.RenderChunks(device, shader);
		}
	}
};
