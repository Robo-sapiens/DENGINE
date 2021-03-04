
#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "model.h"
#include "camera.h"

class Object {
public:
    Object(const glm::vec3 position, Model &model, const glm::vec3 scale = glm::vec3(1.0f)) :
            position_(position),
            model_(&model),
            scale_(scale),
            model_matrix_(glm::scale(glm::translate(glm::mat4(1.0f), position), scale)) {};

    void Draw(const Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const;

    void setScale(glm::vec3);

    void setPos(glm::vec3);

    [[nodiscard]] glm::vec3 getPos() const { return position_; }

    [[nodiscard]] const glm::mat4 &getModel() const { return model_matrix_; }

private:
    glm::vec3 position_;
    glm::vec3 scale_;
    glm::mat4 model_matrix_;

    Model *model_;
};


void Object::Draw(const Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const {
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix_));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    model_->Draw(shader);
}

void Object::setPos(glm::vec3 position) {
    position_ = position;
    model_matrix_ = glm::scale(glm::translate(glm::mat4(1.0f), position_), scale_);
}

void Object::setScale(glm::vec3 scale) {
    scale_ = scale;
    model_matrix_ = glm::scale(glm::translate(glm::mat4(1.0f), position_), scale_);
}


#endif //DRAW_OBJECT_H
