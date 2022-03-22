#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

uniform mat4 u_project;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 v_normal;
out vec3 v_pos;


void main() {

    gl_Position = u_project * u_view * u_model * vec4(aPos, 1);
    v_normal = mat3(transpose(inverse(u_model))) * aNormal;
    v_pos = (u_model * vec4(aPos, 1)).xyz;
}
