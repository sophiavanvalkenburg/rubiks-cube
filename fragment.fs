#version 330 core
in vec3 color;
uniform float isSelected;
uniform float isTapped;

out vec4 out_color;

void main()
{
    float red = (1.0 - isTapped) * color.r + isSelected * 0.75;
    float green = (1.0 - isTapped) * color.g + isSelected * 0.75;
    float blue = (1.0 - isTapped) * color.b + isSelected * 0.75;
    vec3 newColor = vec3(min(1.0, red), min(1.0, green), min(1.0, blue));
    out_color = vec4(newColor, 1.0); 
}
