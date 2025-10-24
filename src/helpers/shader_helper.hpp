#ifndef SHADER_HELPER
#define SHADER_HELPER
#include "helpers_common.hpp"

struct ShaderModule {
	GLuint shader_module;
	GLenum shader_stage_type;
	std::string path_to_shader_dir;
	std::string shader_name_file;
};


struct LineInfo {
	int local_line_id;
	std::string shader_name_file;
};
class ShaderGLSL {

public:
	ShaderGLSL(std::string shader_program_name = "");
	void add_shader(GLenum gl_shader_stage_type, std::string folder_root, std::string shader_name);
	void remove_shader_stage(GLenum gl_shader_stage_type);
	void compile_and_link_to_program();
	void use_shader_program()const;
	void read_file_to_str_array(const std::string& filePath, std::vector<char*>& line_array, std::vector<int>& char_per_line_string);
	void free_string_array_memory(std::vector<char*>& line_array);
	void print_info_log(char* log, std::vector<LineInfo>& lines_info);
	std::string get_include_line(char* test_line);
	void parse_includes(ShaderModule& shader_info,std::vector<char*>& line_array, std::vector<int>& char_per_line_string,std::vector<LineInfo>& lines_info);
private:
	void clear_shader_compilation_state();
	bool try_compile_and_link_to_program();
	bool m_is_shader_program_linked;
	GLuint m_shader_program;
	std::string m_shader_program_name;
	std::vector<ShaderModule> m_shader;
};
#endif