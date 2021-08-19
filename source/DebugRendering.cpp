#include "DebugRendering.h"

#include "File.h"

DebugRendering::DebugRendering(UniformBuffer* camub)
{
	shader = Shader::FromSource(nullptr,
		File::ReadAllText("shaders/screen.vert").data(),
		File::ReadAllText("shaders/screen.frag").data());

	shader->SetUniformBuffer("camera", camub);
}

void DebugRendering::DrawLine(Device* device, glm::vec3 start, glm::vec3 end, glm::vec3 color)
{
	if (vb) 
	{
		delete vb;
		vb = nullptr;
	}

	auto pushvector = [](std::vector<float>& data, glm::vec3 vector)
	{
		data.push_back(vector.x);
		data.push_back(vector.y);
		data.push_back(vector.z);
	};

	pushvector(vertexdata, start);
	pushvector(vertexdata, color);
	pushvector(vertexdata, end);
	pushvector(vertexdata, color);

	linecount++;

	//vb = new VertexBuffer(nullptr, verts.data(), verts.size() * sizeof(float),
	//	{
	//		{ AttributeType::FLOAT, 3, 6 * sizeof(float), 0 },
	//		{ AttributeType::FLOAT, 3, 6 * sizeof(float), 3 * sizeof(float) }
	//	});

	//device->SetShader(shader);

	//device->DrawLines(vb, 2);
}

void DebugRendering::DrawMesh(std::vector<glm::vec3> vertices, std::vector<uint32_t> indices)
{
}

void DebugRendering::DrawCube(Device* device, glm::vec3 corner, glm::vec3 extents, glm::vec3 color)
{
	glm::vec3 ex(extents.x, 0, 0);
	glm::vec3 ey(0, extents.y, 0);
	glm::vec3 ez(0, 0, extents.z);

	DrawLine(device, corner, corner + ex, color);
	DrawLine(device, corner, corner + ey, color);
	DrawLine(device, corner, corner + ez, color);

	DrawLine(device, corner + ex, corner + ex + ey, color);
	DrawLine(device, corner + ex, corner + ex + ez, color);

	DrawLine(device, corner + ez, corner + ez + ey, color);
	DrawLine(device, corner + ez, corner + ez + ex, color);

	DrawLine(device, corner + ey, corner + ey + ex, color);
	DrawLine(device, corner + ey, corner + ey + ez, color);

	DrawLine(device, corner + ex + ez, corner + ex + ez + ey, color);
	DrawLine(device, corner + ex + ey, corner + ex + ey + ez, color);
	DrawLine(device, corner + ez + ey, corner + ez + ey + ex, color);
}

void DebugRendering::Render(Device* device)
{
	if (vb)
	{
		delete vb;
		vb = nullptr;
	}

	// TODO: copy instead of creating new vb using vb methods
	vb = new VertexBuffer(nullptr, vertexdata.data(), vertexdata.size() * sizeof(float),
		{
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 3 * sizeof(float) }
		});

	device->SetShader(shader);

	device->DrawLines(vb, linecount * 2);

	// reset
	vertexdata.clear();
	linecount = 0;


}