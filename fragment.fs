#version 330 core
in vec3 color;
uniform float isSelected;

out vec4 out_color;

void main()
{
    vec3 newColor = vec3(min(1.0, color.r + isSelected * 0.75), min(1.0, color.g + isSelected * 0.75), min(1.0, color.b + isSelected * 0.75));
    out_color = vec4(newColor, 1.0); 
}
