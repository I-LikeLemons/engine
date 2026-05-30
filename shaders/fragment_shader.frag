#version 410 core

out vec4 FragColor;
in vec4 vCol;
in vec2 TexCoord;
flat in int SampleID;
uniform sampler2D uTextures[16];

void main()
{
    vec4 sampled = texture(uTextures[SampleID], TexCoord);
    FragColor = sampled * vCol;
} 
