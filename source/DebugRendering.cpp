#include "DebugRendering.h"

#include "File.h"

DebugRendering::DebugRendering(UniformBuffer* camub)
{
	points.resize(1024);

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

	std::vector<float> verts;

	pushvector(verts, start);
	pushvector(verts, color);
	pushvector(verts, end);
	pushvector(verts, color);

	vb = new VertexBuffer(nullptr, verts.data(), verts.size() * sizeof(float),
		{
			{ AttributeType::FLOAT, 3, 3 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 3 * sizeof(float), 3 * sizeof(float) }
		});

	device->SetShader(shader);

	device->DrawLines(vb, 2);
}

void DebugRendering::DrawMesh(std::vector<glm::vec3> vertices, std::vector<uint32_t> indices)
{
}

void DebugRendering::Render(Device* device)
{
	VertexBuffer* vb = new VertexBuffer(nullptr, nullptr, 1,
		{
			{ AttributeType::FLOAT, 3, 3 * sizeof(float), 0 }
		});

	IndexBuffer* ib = new IndexBuffer(nullptr, nullptr, 1);

	device->Draw(vb, ib, temp_indices_count);

	// reset
	temp_indices_count = 0;
	temp_vert_count = 0;
}