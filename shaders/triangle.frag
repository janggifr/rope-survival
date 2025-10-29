#version 460

out vec4 color_pixel;

in vec3 color;

void main()
{
//gl_FragCoord.xy;
color_pixel = vec4(color,1.0);
}


