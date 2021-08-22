#pragma once

#include <vector>
#include "glm/vec3.hpp"

#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Device.h"

#include "UniformBuffer.h"

struct Line // TODO: impl
{
	glm::vec3 Start;
	glm::vec3 End;
	glm::vec3 Color;
	float Time;

	Line() : Start(0), End(0), Color(255), Time(5.0f)
	{
	}
};

class DebugRendering
{
public:
	DebugRendering(UniformBuffer* camub);

	std::vector<float> vertexdata;
	uint32_t linecount = 0;

	Shader* shader;


	VertexBuffer* vb = nullptr;
	IndexBuffer* ib = nullptr;

	void DrawLine(Device* device, glm::vec3 start, glm::vec3 end, glm::vec3 color);
	void DrawMesh(std::vector<glm::vec3> vertices, std::vector<uint32_t> indices);
	void DrawCube(Device* device, glm::vec3 corner, glm::vec3 extents, glm::vec3 color);

	void Render(Device* device);
};