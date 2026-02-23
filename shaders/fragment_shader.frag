#version 460 core

out vec4 FragColor;
in vec4 vCol;
in vec2 TexCoord;
in flat int SampleID;
uniform sampler2D uTextures[16];

void main()
{
    vec4 sampled = texture(uTextures[SampleID], TexCoord);
    FragColor = sampled * vCol;
} 
