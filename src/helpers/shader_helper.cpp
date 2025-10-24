#include "shader_helper.hpp"

void ShaderGLSL::clear_shader_compilation_state()  
{  
   // Detach and delete all shaders from the program  
   for (size_t i = 0; i < m_shader.size(); i++)  
   {  
       glDetachShader(m_shader_program, m_shader[i].shader_module);  
       glDeleteShader(m_shader[i].shader_module);  
   }  
   m_shader.clear();  

   // Reset the shader program  
   glDeleteProgram(m_shader_program);  
   m_shader_program = glCreateProgram();  
   m_is_shader_program_linked = false;  
}  

void ShaderGLSL::compile_and_link_to_program()  
{  

   while (!try_compile_and_link_to_program())  
   {  
       pause();  
   }  
   return;  
}

ShaderGLSL::ShaderGLSL(std::string shader_program_name)
{
	m_shader_program = glCreateProgram();
	m_shader_program_name = shader_program_name;
	m_is_shader_program_linked = false;
	m_shader.clear();
}

void ShaderGLSL::add_shader(GLenum gl_shader_stage_type, std::string folder_root, std::string shader_name)
{
	//check that no shader of this type is already added
	for (int i = 0; i < m_shader.size(); i++)
	{
		if (m_shader[i].shader_stage_type == gl_shader_stage_type)
		{
			std::cout << "WARNING: shader of type " << gl_shader_stage_type << " already added to shader program " << m_shader_program_name << std::endl;
			std::cout << "         Ignoring shader " << shader_name << std::endl;
			return;
		}
	}
	m_is_shader_program_linked = false;
	ShaderModule shader_module;
	shader_module.shader_stage_type = gl_shader_stage_type;
	shader_module.path_to_shader_dir = ROOT_FOLDER;
	shader_module.shader_name_file = folder_root + "/" + shader_name;
	shader_module.shader_module = glCreateShader(gl_shader_stage_type);
	DebugOpenGL::set_label(GL_SHADER, shader_module.shader_module, m_shader_program_name);
	m_shader.push_back(shader_module);

}

void ShaderGLSL::remove_shader_stage(GLenum gl_shader_stage_type)
{
	m_is_shader_program_linked = false;
	for (int i = 0; i < m_shader.size(); i++)
	{
		if (m_shader[i].shader_stage_type == gl_shader_stage_type)
		{
			glDeleteShader(m_shader[i].shader_module);
			m_shader.erase(m_shader.begin() + i);
		}
	}
}


void ShaderGLSL::free_string_array_memory(std::vector<char*>& line_array) 
{
	for (int i = 0; i < line_array.size(); ++i) {
		delete[] line_array[i];
	}
	line_array.clear();
}

void ShaderGLSL::read_file_to_str_array(const std::string& filePath, std::vector<char*>& line_array, std::vector<int>& char_per_line_string)
	{
	line_array.resize(0);
	char_per_line_string.resize(0);
	std::ifstream shader_file;
	std::vector<std::string> lines;
	std::string tempLine;
	shader_file.open(filePath);
	if (shader_file.is_open())
	{
		// Read the file line by line
		while (!shader_file.eof()) {
			std::getline(shader_file, tempLine);
			lines.push_back(tempLine);
			char_per_line_string.push_back(tempLine.size() + 1);
		}
		shader_file.close();
	}
	else
	{
		std::cout << "ERROR: Could not read " << filePath << std::endl;
		return;
	}
	// Copy lines into newly allocated memory
	for (size_t i = 0; i < char_per_line_string.size(); ++i)
	{
		char* ptr = new char[char_per_line_string[i]+1];
		std::strcpy(ptr, (lines[i] + "\n").c_str());
		line_array.push_back(ptr);
	}

}

std::string ShaderGLSL::get_include_line(char* test_line)
{
	static char fileName[256]; // Static buffer for the file name
	// Clear the buffer
	std::fill(std::begin(fileName), std::end(fileName), 0);

	// Attempt to extract the file name into the buffer
	if (sscanf(test_line, "#include \"%255[^\"]\"", fileName) == 1) {
		// Successfully extracted the file name
		return std::string(fileName);
	}
	else {
		// Pattern not matched
		return std::string("");
	}
}


void ShaderGLSL::parse_includes(ShaderModule& shader_info, std::vector<char*>& line_array, std::vector<int>& char_per_line_string, std::vector<LineInfo>& lines_info)
{
	std::string path = shader_info.path_to_shader_dir;
	std::string root_filename = shader_info.shader_name_file;

	
	std::vector<char*> line_array_in(line_array);
	std::vector<int> char_per_line_string_in(char_per_line_string);

	line_array.clear();
	char_per_line_string.clear();
	lines_info.clear();
	int line_count = 0;

	for (int i = 0; i < line_array_in.size(); i++)
	{
		std::string include_filename = get_include_line(line_array_in[i]);
		if (include_filename.size() > 0)//The tested line is an include
		{
			std::vector<char*> line_array_include;
			std::vector<int> char_per_line_string_include;
			read_file_to_str_array(path + include_filename, line_array_include, char_per_line_string_include);
			for (int j = 0; j < line_array_include.size(); j++)
			{
				if (strstr(line_array_include[j], "//[CPU_ONLY]") == nullptr)
				{
					line_array.push_back(line_array_include[j]);
					char_per_line_string.push_back(char_per_line_string_include[j]);
					LineInfo current_line;
					current_line.local_line_id = j;
					current_line.shader_name_file = include_filename;
					lines_info.push_back(current_line);
				}
			}
		}
		else
		{
			line_array.push_back(line_array_in[i]);
			char_per_line_string.push_back(char_per_line_string_in[i]);
			LineInfo current_line;
			current_line.local_line_id = i;
			current_line.shader_name_file = root_filename;
			lines_info.push_back(current_line);
		}
	}

	//print match table
	/*
	for (int i = 0; i < line_array.size(); i++)
	{
		std::cout << "GID: " << i+1 << " LID: " << lines_info[i].local_line_id+1 << "  "<< lines_info[i].shader_name_file << std::endl;
	}*/
}

void ShaderGLSL::print_info_log(char* log, std::vector<LineInfo>& lines_info)
{
	std::string multilineString = std::string(log);
	std::istringstream iss(multilineString);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(iss, line)) {
		// Add the line to the vector
		if (line != "")
			lines.push_back(line);
	}

	// Print each line
	for (const auto& l : lines) {
		try {
			// Find positions for parsing
			size_t startPos = l.find('(');
			size_t endPos = l.find(')');
			if (startPos == std::string::npos || endPos == std::string::npos || endPos <= startPos) {
				throw std::runtime_error("Invalid format: missing parentheses");
			}
			startPos++; // move after '('

			std::string errorMessage = l.substr(l.find(':') + 2);
			std::string lineNumberStr = l.substr(startPos, endPos - startPos);

			int lineNumber = std::stoi(lineNumberStr); // may throw

			if (lineNumber < 0 || lineNumber >= static_cast<int>(lines_info.size())) {
				throw std::out_of_range("Line number out of range");
			}

			std::cout << lines_info[lineNumber].shader_name_file
				<< ", line " << lines_info[lineNumber].local_line_id
				<< ", " << errorMessage << std::endl;
		}
		catch (const std::exception& e) {
			std::cout << l << std::endl;
		}
	}
}

bool ShaderGLSL::try_compile_and_link_to_program()
{
	int success;
	char infoLog[8096];
	std::ifstream shader_file;

	//compile shaders
	for (size_t i = 0; i < m_shader.size(); i++)
	{
		std::vector<int> file_line_alloc;
		std::vector<char*> string_array;
		std::vector<LineInfo> lines_info;
		read_file_to_str_array(m_shader[i].path_to_shader_dir + m_shader[i].shader_name_file, string_array, file_line_alloc);
		parse_includes(m_shader[i],string_array, file_line_alloc, lines_info);

		//Send code to GPU compiler
		glDeleteShader(m_shader[i].shader_module);
		m_shader[i].shader_module = glCreateShader(m_shader[i].shader_stage_type);
		glShaderSource(m_shader[i].shader_module, file_line_alloc.size(), string_array.data(), file_line_alloc.data());
		free_string_array_memory(string_array);

		glCompileShader(m_shader[i].shader_module);

		glGetShaderiv(m_shader[i].shader_module, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(m_shader[i].shader_module, 8096, nullptr, infoLog);
			std::cout << "ERROR: Failed to compile " << m_shader[i].shader_name_file << " :" << std::endl;
			print_info_log(infoLog, lines_info);


			return false;
		}

	}

	//link shader program
	glDeleteProgram(m_shader_program);
	m_shader_program = glCreateProgram();
	DebugOpenGL::set_label(GL_PROGRAM, m_shader_program, m_shader_program_name);
	for (int i = 0; i < m_shader.size(); i++)
	{
		glAttachShader(m_shader_program, m_shader[i].shader_module);
	}
	glLinkProgram(m_shader_program);

	glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shader_program, 8096, nullptr, infoLog);
		std::cout << "ERROR: Failed to link " << m_shader_program_name << " :" << std::endl;
		std::cout << infoLog << std::endl;
		glDeleteProgram(m_shader_program);
		return false;
	}

	for (int i = 0; i < m_shader.size(); i++)
	{
		glDetachShader(m_shader_program, m_shader[i].shader_module);
	}

	std::cout << "Successfully compiled and linked " << m_shader_program_name << std::endl;
	m_is_shader_program_linked = true;


	//validate shader program
	glValidateProgram(m_shader_program);
	glGetProgramiv(m_shader_program, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shader_program, 8096, nullptr, infoLog);
		std::cout << "WARNING: Failed to validate " << m_shader_program_name << " :" << std::endl;
		std::cout << infoLog << std::endl;
	}

	return true;
}

void ShaderGLSL::use_shader_program() const
{
	if (m_is_shader_program_linked)
		glUseProgram(m_shader_program);
	else
		std::cout << "ERROR: " << m_shader_program_name << " not compiled/linked !" << std::endl;
}
