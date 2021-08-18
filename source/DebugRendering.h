#pragma once

#include <vector>
#include "glm/vec3.hpp"

#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Device.h"

#include "UniformBuffer.h"

class DebugRendering
{
public:
	DebugRendering(UniformBuffer* camub);

	std::vector<glm::vec3> points;

	Shader* shader;

	uint32_t temp_vert_count = 0;
	uint32_t temp_indices_count = 0;

	VertexBuffer* vb = nullptr;
	IndexBuffer* ib = nullptr;

	void DrawLine(Device* device, glm::vec3 start, glm::vec3 end, glm::vec3 color);
	void DrawMesh(std::vector<glm::vec3> vertices, std::vector<uint32_t> indices);

	void Render(Device* device);
};