#version 330 core

layout (location = 0) in vec3 loc_pos;

uniform mat4 clip_matrix;

void main()
{
	gl_Position = clip_matrix * vec4(loc_pos, 1.0f);
}
