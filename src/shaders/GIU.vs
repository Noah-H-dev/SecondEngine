#version 410 core
layout (location = 0) in vec3 aPos; // The input vertex position
out vec3 Color;
uniform vec3 aColor;
uniform mat4 projection;
void main()
{
    gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0); // Output the vertex position
    Color = aColor;
}