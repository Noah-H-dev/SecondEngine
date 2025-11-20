#version 410 core
out vec4 FragColor; // The output fragment color

in vec3 Color;

void main()
{
    FragColor = vec4(Color.x, Color.y, Color.z, 1.0f); // Output an orange color
}