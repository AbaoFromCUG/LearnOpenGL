#version 330 core

in vec3 v_normal;
in vec3 v_pos;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_cameraPos;

out vec4 glFragColor;



void main() {
    // ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    // diffuse light
    vec3 normal= normalize(v_normal);
    vec3 lightDir = normalize(u_lightPos - v_pos);
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * u_objectColor;

    // specular light
    float specularStrength = 0.5;
    float shininess = 32;
    vec3 viewDir = normalize(u_cameraPos - v_pos);
    vec3 reflectDir = reflect(-lightDir, v_normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), shininess);
    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    glFragColor = vec4(result, 1);
}
