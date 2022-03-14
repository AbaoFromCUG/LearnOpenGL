// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>

// clang-format off
float vertices[] ={
    -0.5, 0.5,0,  0, 0,
    0.5, 0.5,0,   1, 0,
    0.5, -0.5,0,   1, 1,
    -0.5, -0.5,0,   0, 1
};

unsigned int indies[] = {
    0, 1, 2,
    2, 3, 0
};

// clang-format on

void processEvent(GLFWwindow *window);

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

glm::mat4 getTransform();

glm::vec3 scale(1);
glm::vec2 translate(0, 0);
bool isMoving = false;
double lastPosX = -1;
double lastPosY = -1;

double windowWidth = 500;
double windowHeight = 600;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    std::cout << "apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    auto window = glfwCreateWindow(windowWidth, windowHeight, "Popup-transformations", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    int width, height;
    int channel;
    unsigned char *data = stbi_load("res/smile.jpg", &width, &height, &channel, 0);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indies), indies, GL_STATIC_DRAW);

    auto shader = Shader::fromPath("res/shader/get_started/transformations.vs", "res/shader/get_started/transformations.fs");

    glClearColor(0.2, 0.4, 0.5, 1);
    while (!glfwWindowShouldClose(window)) {
        processEvent(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.setInt("u_texture", 0);
        shader.setMat4("transform", getTransform());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void processEvent(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    scale = scale + glm::vec3(yoffset * 0.1);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isMoving = true;
        } else if (action == GLFW_RELEASE) {
            isMoving = false;
            lastPosX = -1;
            lastPosY = -1;
        }
    }
}

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    if (isMoving) {
        if (lastPosX >= 0) {
            double deltaX = xpos - lastPosX;
            double deltaY = ypos - lastPosY;
            // opengl NDC size(2, 2), map to window size(windowWidth, windowHeight);
            translate = translate + glm::vec2(deltaX / windowWidth * 2, -deltaY / windowHeight * 2);
        }
        lastPosX = xpos;
        lastPosY = ypos;
    }
}

glm::mat4 getTransform() {
    glm::mat4 transform = glm::mat4(1);
    transform = glm::translate(transform, glm::vec3(translate, 0));
    if (!isMoving){
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0, 0, 1));
    }
    transform = glm::scale(transform, scale);
    return transform;
}
