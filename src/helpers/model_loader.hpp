#pragma once
#include "helpers_common.hpp"
#include <tiny_obj_loader.h>

struct ModelDataStructure
{
	GPUBuffer vertex_buffer; //buffer for vertex positions (vec4) input, constant
	GPUBuffer tri_index_buffer; //buffer for triangle index (uvec4)
	uint32_t triangle_count; //number of triangles in the model
	uint32_t vertex_count; //number of vertices in the model
};

void load_model(ModelDataStructure &model, std::string base_name);

void bind_model_ssbos(ModelDataStructure& model,GLuint ssbo_tri_id, GLuint ssbo_vert_id);