#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

uniform mat4 u_project;
uniform mat4 u_view;
uniform mat4 u_model;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_cameraPos;


out vec3 v_color;


void main() {
    gl_Position = u_project * u_view * u_model * vec4(aPos, 1);
    vec3 position = (u_model * vec4(aPos, 1)).xyz;

    // ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    // diffuse light
    mat3 normalModelMatrix = mat3(transpose(inverse(u_model)));
    vec3 normal =  normalize(normalModelMatrix * aNormal);
    vec3 lightDir = normalize(u_lightPos - position);
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * u_lightColor;

    // specular light
    float specularStrength = 0.5;
    float shininess = 32;
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(u_cameraPos - position);
    float spec = pow(max(dot(reflectDir, viewDir), 0), shininess);
    vec3 specular = specularStrength * spec  * u_lightColor;



    v_color = (ambient + diffuse + specular) * u_objectColor;
}
