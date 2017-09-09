#version 330 core
layout (location = 0) in  vec3 position;
layout (location = 1) in vec2 in_tex_coord;

out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    tex_coord = vec2(in_tex_coord.x, 1.0f - in_tex_coord.y); // invert the y because images are flipped
}
