#version 330 core

in vec3 fragColor;
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform float time;

void main()
{
    float shift = 0.08 * sin(time + fragPos.y * 4.0);
    vec3 color = fragColor + vec3(shift);
    FragColor = vec4(color, 1.0);
}
