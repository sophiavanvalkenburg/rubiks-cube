#version 330 core
in vec2 tex_coord;

out vec4 out_color;

uniform sampler2D uni_texture1;
uniform sampler2D uni_texture2;
uniform float uni_texture_mix_amt;

void main()
{
    out_color = mix(texture(uni_texture1, tex_coord), texture(uni_texture2, tex_coord), uni_texture_mix_amt);
}
