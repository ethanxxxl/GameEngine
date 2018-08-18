#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 clip_matrix;

void main()
{
	gl_Position = clip_matrix * vec4(aPos, 1.0f);
}
