// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "player_camera.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// clang-format off
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0, 0, -1,
    0.5f, -0.5f, -0.5f,  0, 0, -1,
    0.5f,  0.5f, -0.5f,  0, 0, -1,
    0.5f,  0.5f, -0.5f,  0, 0, -1,
    -0.5f,  0.5f, -0.5f,  0, 0, -1,
    -0.5f, -0.5f, -0.5f,  0, 0, -1,

    -0.5f, -0.5f,  0.5f,  0, 0, 1,
    0.5f, -0.5f,  0.5f,  0, 0, 1,
    0.5f,  0.5f,  0.5f,  0, 0, 1,
    0.5f,  0.5f,  0.5f, 0, 0, 1,
    -0.5f,  0.5f,  0.5f, 0, 0, 1,
    -0.5f, -0.5f,  0.5f,  0, 0, 1,

    -0.5f,  0.5f,  0.5f,  -1, 0, 0,
    -0.5f,  0.5f, -0.5f,  -1, 0, 0,
    -0.5f, -0.5f, -0.5f,  -1, 0, 0,
    -0.5f, -0.5f, -0.5f,  -1, 0, 0,
    -0.5f, -0.5f,  0.5f,  -1, 0, 0,
    -0.5f,  0.5f,  0.5f,  -1, 0, 0,

    0.5f,  0.5f,  0.5f,  1, 0, 0,
    0.5f,  0.5f, -0.5f,  1, 0, 0,
    0.5f, -0.5f, -0.5f,  1, 0, 0,
    0.5f, -0.5f, -0.5f,  1, 0, 0,
    0.5f, -0.5f,  0.5f,  1, 0, 0,
    0.5f,  0.5f,  0.5f,  1, 0, 0,

    -0.5f, -0.5f, -0.5f, 0, -1, 0,
    0.5f, -0.5f, -0.5f,  0, -1, 0,
    0.5f, -0.5f,  0.5f,  0, -1, 0,
    0.5f, -0.5f,  0.5f,  0, -1, 0,
    -0.5f, -0.5f,  0.5f,  0, -1, 0,
    -0.5f, -0.5f, -0.5f,  0, -1, 0,

    -0.5f,  0.5f, -0.5f,  0, 1 , 0,
    0.5f,  0.5f, -0.5f,   0, 1 , 0,
    0.5f,  0.5f,  0.5f,  0, 1 , 0,
    0.5f,  0.5f,  0.5f,  0, 1 , 0,
    -0.5f,  0.5f,  0.5f,  0, 1 , 0,
    -0.5f,  0.5f, -0.5f,  0, 1 , 0,
};
// clang-format on

void frameBufferSizeCallback(GLFWwindow *window, int width, int heigh);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

int windowWidth = 800;
int windowHeight = 500;

PlayerCamera playerCamera;

float deltaTime = 0;
float lastTime = 0;

float lastMouseX = 0;
float lastMouseY = 0;
bool isFirstMouse = true;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    std::cout << "apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif


    auto window = glfwCreateWindow(600, 400, "Popup-BasicLighting", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    auto shader = Shader::fromPath("res/shader/lighting/basic_lighting.vs", "res/shader/lighting/basic_lighting.fs");
    lastTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4, 0.5, 0.6, 1);
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::vec3 objectColor = glm::vec3(1, 0.5, 0.2);
        glm::vec3 lightColor = glm::vec3(1, 1, 1);
        glm::vec3 lightPos = glm::vec3(glm::sin(glfwGetTime()), 2, 2);
        shader.setVec3("u_objectColor", objectColor);
        shader.setVec3("u_lightColor", lightColor);
        shader.setVec3("u_lightPos", lightPos);
        shader.setVec3("u_cameraPos", playerCamera.position());
        glBindVertexArray(VAO);

        glm::mat4 project = glm::perspective<float>(glm::radians(45.0), float(windowWidth) / windowHeight, 0.1f, 100);
        shader.setMat4("u_project", project);
        shader.setMat4("u_view", playerCamera.getViewMatrix());
        shader.setMat4("u_model", glm::mat4(1));

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int heigh) {
    glViewport(0, 0, width, heigh);
    windowWidth = width;
    windowHeight = heigh;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_W) {
        playerCamera.move(PlayerCamera::Movement::FORWARD, deltaTime);
    } else if (key == GLFW_KEY_S) {
        playerCamera.move(PlayerCamera::Movement::BACKWARD, deltaTime);
    } else if (key == GLFW_KEY_A) {
        playerCamera.move(PlayerCamera::Movement::LEFT, deltaTime);
    } else if (key == GLFW_KEY_D) {
        playerCamera.move(PlayerCamera::Movement::RIGHT, deltaTime);
    } else if (key == GLFW_KEY_E) {
        playerCamera.move(PlayerCamera::Movement::UP, deltaTime);
    } else if (key == GLFW_KEY_Q) {
        playerCamera.move(PlayerCamera::Movement::DOWN, deltaTime);
    }
}

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    if (isFirstMouse) {
        isFirstMouse = false;
    } else {
        playerCamera.rotate((float)xpos - lastMouseX, lastMouseY - ypos);
    }

    lastMouseX = (float)xpos;
    lastMouseY = (float)ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {}
