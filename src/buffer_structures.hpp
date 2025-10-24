//This files defines structures and binding points for GPU shaders, as well as CPU layout
//Do not edit "[CPU_ONLY]" as they are parsed when compiling shaders
#ifndef BUFFER_STRUCTURES //[CPU_ONLY]
#define BUFFER_STRUCTURES //[CPU_ONLY]
#include <glm/glm.hpp> //[CPU_ONLY]
#define uint uint32_t //[CPU_ONLY] for compatibility with GLSL
using namespace glm; //[CPU_ONLY]


//structures for uniform buffers (UBOs) require proper 16 byte alignment (std140 layout)
//structures for shader storage buffers (SSBOs) require only 4 byte alignment (std430 layout)
//more at https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout


#define UBO_APPLICATION_BINDING 0
struct ApplicationUboDataStructure
{
	//
};

#endif //[CPU_ONLY]