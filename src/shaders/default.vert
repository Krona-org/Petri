#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

// mat4 занимает 4 локации подряд
layout(location = 3) in mat4 iModel;

out vec3 fragColor;
out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool useInstancing;

uniform vec3 objectColor;
uniform bool useObjectColor;

void main()
{
    mat4 M = useInstancing ? iModel : model;

    fragColor = useObjectColor ? objectColor : aColor;

    vec4 worldPos = M * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;

    normal = mat3(transpose(inverse(M))) * aNormal;

    gl_Position = projection * view * worldPos;
}
