
#ifndef DRAW_CAMERA_H
#define DRAW_CAMERA_H


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera {
public:
    Camera(glm::vec3 pos, GLint screenWidth, GLint screenHeight) :
            pos(pos),
            front(glm::vec3(0.0f, 0.0f, -1.0f)),
            up(glm::vec3(0.0f, 1.0f, 0.0f)),
            view(glm::mat4(1.0f)),
            projection(glm::mat4(1.0f)),
            width(static_cast<float>(screenWidth)),
            height(static_cast<float>(screenHeight)),
            cursorY(static_cast<float>(screenHeight) / 2),
            cursorX(static_cast<float>(screenWidth) / 2),
            keys(std::vector<bool>(1024, false))
            {};

    void keyboard(GLint key, GLint action);

    void cursor(GLdouble xpos, GLdouble ypos);

    void scroll(GLdouble yoffset);

    void update(const Shader &shader);

    [[nodiscard]] const glm::mat4 &getView() const { return view; }

    [[nodiscard]] const glm::mat4 &getProjection() const { return projection; }

    [[nodiscard]] const glm::vec3 &getFront() const { return front; }

    [[nodiscard]] const glm::vec3 &getPosition() const { return pos; }

private:
    GLfloat yaw = -90.0f;
    GLfloat pitch = 0.0f;
    GLfloat speed = 0.015f;
    GLfloat sense = 0.05f;
    GLfloat zoom = 45.0f;

    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 projection;

    GLfloat cursorX;
    GLfloat cursorY;

    GLfloat width;
    GLfloat height;

    std::vector<bool> keys;
};

void Camera::cursor(GLdouble xpos, GLdouble ypos) {
    GLfloat dx = xpos - cursorX;
    GLfloat dy = cursorY - ypos;
    cursorX = xpos;
    cursorY = ypos;

    yaw += dx * sense;
    pitch += dy * sense;

    if (pitch > 89.9f)
        pitch = 89.9f;
    if (pitch < -89.9f)
        pitch = -89.9f;

    glm::vec3 _front;
    _front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    _front.y = sin(glm::radians(pitch));
    _front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(_front);
}

void Camera::scroll(GLdouble yoffset) {
    if(zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if(zoom <= 1.0f)
        zoom = 1.0f;
    if(zoom >= 45.0f)
        zoom = 45.0f;
}

void Camera::update(const Shader &shader) {
    glUniform3f(glGetUniformLocation(shader.program, "viewPos"), pos.x, pos.y, pos.z);

    projection = glm::perspective(glm::radians(zoom), width / height, 0.1f, 100.0f);
    view = glm::lookAt(pos, pos + front, up);


    if (keys[GLFW_KEY_W]) {
        pos += speed * front;
    }

    if (keys[GLFW_KEY_S]) {
        pos -= speed * front;
    }

    if (keys[GLFW_KEY_A]) {
        pos -= glm::normalize(glm::cross(front, up)) * speed;
    }

    if (keys[GLFW_KEY_D]) {
        pos += glm::normalize(glm::cross(front, up)) * speed;
    }

    if (keys[GLFW_KEY_SPACE]) {
        pos.y += speed;
    }

    if (keys[GLFW_KEY_LEFT_CONTROL]) {
        pos.y -= speed;
    }

    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        speed = 0.03;
    } else {
        speed = 0.015;
    }
}

void Camera::keyboard(GLint key, GLint action) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}


#endif //DRAW_CAMERA_H
