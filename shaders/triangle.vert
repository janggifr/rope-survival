#version 460


layout(std430, binding = 0) buffer NAME
{
	vec4 vertices[];
};

layout(std430, binding = 1) buffer NAME2
{
	vec4 color_vert[];
};

out vec3 color;

void main()
{
int id = gl_VertexID % 3;

float time = color_vert[0].w;
color = color_vert[id].xyz;

vec2 pos = vertices[id].xy;

float pid = float(gl_VertexID/3);

float a = 4.0*pid*3.1415/180.0 + 0.0*time;
float ca = cos(a);
float sa = sin(a);

mat2 rot = mat2(ca,sa,-sa,ca);

pos = rot * pos;

gl_Position = vec4(pos,0.0,1.0);
}