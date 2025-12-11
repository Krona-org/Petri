#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

out vec3 fragColor;
out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fragColor = aColor; // передаем цвет из VBO

    vec4 worldPos = model * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;

    normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * worldPos;
}
