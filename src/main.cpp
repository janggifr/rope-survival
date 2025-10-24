#include "helpers/impl_defines.hpp"
#include "helpers/helpers_common.hpp"
#include "buffer_structures.hpp"
#include "helpers/model_loader.hpp"



int main(int argc, char* argv[]) 
{
	/////////////////////////////IGNORE THIS SECTION OF CODE/////////////////////////////////
	ContextHelper::init_context_all(800, 600, "Rope Survival",4);
	ContextHelper::print_opengl_info();
	

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

