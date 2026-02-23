#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in int aSamp;

uniform mat4 u_transform;
uniform vec2 viewport;
out vec4 vCol;
out vec2 TexCoord;
flat out int SampleID;
void main() {
    gl_Position = u_transform * vec4(aPos, 1.0);
    vCol = aCol;
    TexCoord = aTexCoord;
    SampleID = int(aSamp + 0.5);
}
