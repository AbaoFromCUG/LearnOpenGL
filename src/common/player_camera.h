//
// Created by Abao Zhang on 2022/3/17.
//

#ifndef LEARNOPENGL_PLAYER_CAMERA_H
#define LEARNOPENGL_PLAYER_CAMERA_H

#include <glm/glm.hpp>

class PlayerCamera {
public:
    enum class Movement {
        FORWARD,  // w
        BACKWARD, // s
        LEFT,     // a
        RIGHT,    // d
        UP,       // r
        DOWN,     // e
    };

public:
    PlayerCamera();

public:
    const glm::vec3 &position() const { return m_cameraPos; }

public:
    void move(Movement direction, float deltaTime);
    void rotate(float deltaX, float deltaY);

    const glm::mat4 &getViewMatrix() const;

private:
    void updateViewMatrix();

private:
    glm::vec3 m_cameraPos;
    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;
    float m_cameraPitch;
    float m_cameraYaw;

    glm::mat4 m_cameraViewMatrix;

    // camera options
    float m_movementSpeed = 25;
    float m_mouseSensitivity = 0.1;
};

#endif // LEARNOPENGL_PLAYER_CAMERA_H
