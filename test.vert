#version 330 core
layout (location = 0) in vec3 vPos;

uniform mat4 model;
uniform mat4 projection;

void main() {
    // gl_Position = projection * model * vec4(vPos, 1.0);
    gl_Position = vec4(vPos, 1.0);
}