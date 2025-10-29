#include "helpers/impl_defines.hpp"
#include "helpers/helpers_common.hpp"
#include "buffer_structures.hpp"
#include "helpers/model_loader.hpp"



int main(int argc, char* argv[])
{

	std::vector<float> arr;
	arr.resize(5);

	arr[0] = 4.5f;
	arr[1] = -4.5f;
	arr[2] = 0.3f;
	arr[3] = 1.1f;
	arr[4] = 4.0f;


	
	/////////////////////////////IGNORE THIS SECTION OF CODE/////////////////////////////////
	ContextHelper::init_context_all(800, 600, "Rope Survival",4);
	ContextHelper::print_opengl_info();
	

	GPUBuffer my_gpu_buffer;
	my_gpu_buffer.allocate(sizeof(float) * arr.size());

	my_gpu_buffer.write_to_gpu(arr.data(), 0, sizeof(float) * arr.size());

	std::vector<float> arr_back;
	arr_back.resize(5);

	float clear_value = 9.9f;
	//my_gpu_buffer.clear_to_value({ GL_R32F,GL_RED,GL_FLOAT,1 }, &clear_value);
	my_gpu_buffer.write_to_gpu(&clear_value, 3* sizeof(float), sizeof(float));

	my_gpu_buffer.read_from_gpu(arr_back.data(), 0, sizeof(float) * arr_back.size());

	std::cout << arr_back[0] << " " << arr_back[1] << " " << arr_back[2] << " " << arr_back[3] << " " << arr_back[4] << std::endl;


	exit(0);


	while (ContextHelper::should_not_close_window()) //main/render loop
	{
		ContextHelper::begin_frame();//Prepare frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Ignore
		if (ContextHelper::window_resized)// Graphics rendering, ignore
		{
			glViewport(0, 0, ContextHelper::resolution.x, ContextHelper::resolution.y);//ignore
		}



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

