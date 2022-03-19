// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "player_camera.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stb_image.h>

// clang-format off
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
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

    auto window = glfwCreateWindow(windowWidth, windowHeight, "Popup-Camera", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, channel;
    unsigned char *data = stbi_load("res/wall.jpg", &width, &height, &channel, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    auto shader = Shader::fromPath("res/shader/get_started/coordinate_system.vs", "res/shader/get_started/coordinate_system.fs");
    lastTime = glfwGetTime();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4, 0.5, 0.6, 1);
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.setInt("u_texture", 0);

        for (int i = 0; i < 10; ++i) {
            glm::mat4 projection = glm::mat4(1);
            projection = glm::perspective(glm::radians(40.0f), float(windowWidth) / float(windowHeight), 0.1f, 100.0f);
            shader.setMat4("u_projection", projection);

            shader.setMat4("u_view", playerCamera.getViewMatrix());

            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, cubePositions[i]);
            float degree = float(i * 20);
            model = glm::rotate(model, degree, glm::vec3(0.5, 1, 1));
            shader.setMat4("u_model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
