#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec4 normal;
out vec4 frag_pos;

uniform mat4 clip_matrix;

void main()
{
	gl_Position = clip_matrix * vec4(a_pos, 1.0f);
	frag_pos = vec3(model * vec4(a_pos, 1.0));
	normal = a_normal;
}
