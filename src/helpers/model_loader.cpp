
#include "model_loader.hpp"

void load_model(ModelDataStructure& model, std::string base_name)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (ROOT_FOLDER + base_name).c_str());
	if (!warn.empty()) {
		std::cout << "Warn: " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << "Err: " << err << std::endl;
	}

	if (!ret) {
		std::cerr << "Failed to load/parse .obj file" << std::endl;
		return;
	}

	std::vector<glm::vec4> vertex_data;
	model.vertex_count = attrib.vertices.size() / 3;
	vertex_data.resize(model.vertex_count);

	for (size_t v = 0; v < model.vertex_count; v++) {
		vertex_data[v].x = -attrib.vertices[3 * v + 0];
		vertex_data[v].y = attrib.vertices[3 * v + 1];
		vertex_data[v].z = attrib.vertices[3 * v + 2];
		vertex_data[v].w = 0.0f;
	}

	std::vector<glm::uvec4> tri_data;

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			if (fv != 3)
			{
				std::cerr << "Not a triangular OBJ" << std::endl;
				return;
			}
			glm::uvec4 tri;
			tri.x = shapes[s].mesh.indices[index_offset + 0].vertex_index;
			tri.y = shapes[s].mesh.indices[index_offset + 1].vertex_index;
			tri.z = shapes[s].mesh.indices[index_offset + 2].vertex_index;
			tri.w = 0u;
			tri_data.push_back(tri);
			index_offset += fv;
		}
	}
	model.triangle_count = tri_data.size();
	model.vertex_buffer.allocate(vertex_data.size() * sizeof(glm::vec4));
	model.vertex_buffer.write_to_gpu(vertex_data.data());
	model.tri_index_buffer.allocate(tri_data.size() * sizeof(glm::uvec4));
	model.tri_index_buffer.write_to_gpu(tri_data.data());

	//Print some info
	std::cout << "Loaded model: " << base_name << " ,  vertex count: " << model.vertex_count << " ,  triangle count: " << model.triangle_count << std::endl;

}


void bind_model_ssbos(ModelDataStructure& model, GLuint ssbo_tri_id, GLuint ssbo_vert_id)
{
	model.tri_index_buffer.set_target_and_slot(GL_SHADER_STORAGE_BUFFER, ssbo_tri_id);
	model.vertex_buffer.set_target_and_slot(GL_SHADER_STORAGE_BUFFER, ssbo_vert_id);
}