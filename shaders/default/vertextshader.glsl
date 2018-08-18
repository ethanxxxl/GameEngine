#version 330 core

layout (location = 0) in loc_pos;

uniform mat4 clip_matrix;

void main()
{
	gl_Position = locPos * clip_matrix
}
