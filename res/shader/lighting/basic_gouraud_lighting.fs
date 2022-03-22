#version 330 core

in vec3 v_color;

out vec4 glFragColor;



void main() {

    glFragColor = vec4(v_color, 1);
}
