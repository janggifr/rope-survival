#include "helpers/impl_defines.hpp"
#include "helpers/helpers_common.hpp"
#include "buffer_structures.hpp"
#include "helpers/model_loader.hpp"



int main(int argc, char* argv[])
{
	ContextHelper::init_context_all(800, 600, "Rope Survival",4);
	ContextHelper::print_opengl_info();
	VertexArrayObject dummy;
	dummy.use_vao();

	ShaderGLSL tri_shader(" triangle");
	tri_shader.add_shader(GL_VERTEX_SHADER, std::string("shaders/"), "triangle.vert");
	tri_shader.add_shader(GL_FRAGMENT_SHADER, std::string("shaders/"), "triangle.frag");
	tri_shader.compile_and_link_to_program();
	ContextHelper::add_shader_to_hot_reload(&tri_shader);

	GPUBuffer triangle_coordinates;

	uint32_t vertex_count = 3;
	triangle_coordinates.allocate(vertex_count*sizeof(vec4));

	std::vector<vec4> vertices;
	vertices.resize(vertex_count);

	vertices[0] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vertices[1] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2] = vec4(1.0f,0.05f, 0.0f, 0.0f);

	triangle_coordinates.write_to_gpu(vertices.data(), 0, vertex_count * sizeof(vec4));
	triangle_coordinates.set_target_and_slot(GL_SHADER_STORAGE_BUFFER, 0);


	GPUBuffer vertex_colors;
	vertex_colors.allocate(vertex_count * sizeof(vec4));

	std::vector<vec4> color;
	color.resize(vertex_count);

	color[0] = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	color[1] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	color[2] = vec4(1.0f, 1.0f, 0.0f, 0.0f);


	vertex_colors.write_to_gpu(color.data(), 0, vertex_count * sizeof(vec4));
	vertex_colors.set_target_and_slot(GL_SHADER_STORAGE_BUFFER, 1);


	while (ContextHelper::should_not_close_window()) //main/render loop
	{
		ContextHelper::begin_frame();//Prepare frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Ignore
		if (ContextHelper::window_resized)// Graphics rendering, ignore
		{
			glViewport(0, 0, ContextHelper::resolution.x, ContextHelper::resolution.y);//ignore
		}

		vec4 data = color[0];
		data.w = ContextHelper::time_from_start_s;
		vertex_colors.write_to_gpu(&data, 0,sizeof(vec4));

		tri_shader.use_shader_program();
		uint32_t tri_count = 90;
		glDrawArrays(GL_TRIANGLES,0, tri_count*3);
		glFlush();



		ImGui::Begin("GUI");//GUI
		if (ImGui::TreeNode("General"))
		{
			static float font_scale = 1.2f;
			ImGui::SetWindowFontScale(font_scale);
			ImGui::SliderFloat("text scale", &font_scale, 0.5f, 4.0f);

			//ImGui::Text("Camera pos: x:%1.2f y:%1.2f z:%1.2f", cam.m_pos.x, cam.m_pos.y, cam.m_pos.z);
			//ImGui::Text("Camera look dir: x:%1.2f y:%1.2f z:%1.2f", cam.m_w_v[0].z, cam.m_w_v[1].z, cam.m_w_v[2].z);
			ImGui::TreePop();
		}
		
		ImGui::End();

		ContextHelper::end_frame();//glfwSwapBuffers [hookpoint for profiler/debugger]
	}

	ContextHelper::destroy_context_all();
	return EXIT_SUCCESS;
}

