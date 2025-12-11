#version 410 core

in vec3 fragColor;
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform float time;

void main()
{
    // Используем цвет из вершин
    vec3 base = fragColor;

    // Опциональная лёгкая анимация цвета
    float shift = 0.1 * sin(time + fragPos.y * 5.0);
    vec3 color = base + vec3(shift);

    FragColor = vec4(color, 1.0); // полностью непрозрачная сфера
}
