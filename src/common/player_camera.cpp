//
// Created by Abao Zhang on 2022/3/17.
//

#include "player_camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

PlayerCamera::PlayerCamera() {
    m_cameraYaw = 0;
    m_cameraPitch = 0;
    m_cameraPos = glm::vec3(0);
    updateViewMatrix();
}

void PlayerCamera::move(PlayerCamera::Movement direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    switch (direction) {
    case Movement::FORWARD: {
        m_cameraPos += m_front * velocity;
        break;
    }
    case Movement::BACKWARD: {
        m_cameraPos -= m_front * velocity;
        break;
    }
    case Movement::LEFT: {
        m_cameraPos -= m_right * velocity;
        break;
    }
    case Movement::RIGHT: {
        m_cameraPos += m_right * velocity;
        break;
    }
    case Movement::UP: {
        m_cameraPos += m_up * velocity;
        break;
    }
    case Movement::DOWN: {
        m_cameraPos -= m_up * velocity;
        break;
    }
    }
    updateViewMatrix();
}

void PlayerCamera::rotate(float deltaX, float deltaY) {
    deltaX *= m_mouseSensitivity;
    deltaY *= m_mouseSensitivity;
    m_cameraPitch += deltaY;
    m_cameraYaw += deltaX;
    m_cameraPitch = glm::clamp(m_cameraPitch, -90.0f, 90.0f);
    updateViewMatrix();
}

const glm::mat4 &PlayerCamera::getViewMatrix() const {
    return m_cameraViewMatrix;
}

void PlayerCamera::updateViewMatrix() {
    m_front.x = glm::cos(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
    m_front.y = glm::sin(glm::radians(m_cameraPitch));
    m_front.z = glm::sin(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
    m_front = glm::normalize(m_front);

    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0, 1, 0)));

    m_up = glm::normalize(glm::cross(m_right, m_front));
    std::cout << "cameraPos: " << m_cameraPos.x << ", " << m_cameraPos.y << ", " << m_cameraPos.z << std::endl;
    m_cameraViewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_front, m_up);
}
