
#ifndef DRAW_LAMP_H
#define DRAW_LAMP_H


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Lamp {
public:
    Lamp(glm::vec3 &&pos,
         glm::vec4 &&color,
         glm::vec3 &&scale = glm::vec3(1.0f)) :
            position_(pos),
            color_(color),
//            form_name_("pointLights[" + std::to_string(number) + "]"),
            model_(glm::scale(glm::translate(glm::mat4(1.0f), pos), scale)) {
//        lamp_uniform_.position = form_name_ + ".position";
//
//        lamp_uniform_.constant = form_name_ + ".constant";
//        lamp_uniform_.linear = form_name_ + ".linear";
//        lamp_uniform_.quadratic = form_name_ + ".quadratic";
//
//        lamp_uniform_.ambient = form_name_ + ".ambient";
//        lamp_uniform_.diffuse = form_name_ + ".diffuse";
//        lamp_uniform_.specular = form_name_ + ".specular";
    };

    static void staticInit(const GLfloat *vertices, GLuint64 ver_size, const GLuint *indices, GLuint64 ind_size);

    static void staticDeinit();

    void draw(const Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const;

    void use(const Shader &shader) const;

    [[maybe_unused]] void set_pos(glm::vec3 &&);

    [[nodiscard]] glm::vec3 get_pos() const { return position_; }

    [[nodiscard]] glm::vec4 get_color() const { return color_; }

    [[nodiscard]] glm::mat4 get_model() const { return model_; }
private:
    struct {
        std::string position;

        std::string constant;
        std::string linear;
        std::string quadratic;

        std::string ambient;
        std::string diffuse;
        std::string specular;
    } lamp_uniform_;

    glm::vec3 position_;
    glm::vec4 color_;
    glm::mat4 model_;

    std::string form_name_;

    static GLuint VBO;
    static GLuint EBO;
    static GLuint VAO;
};

GLuint Lamp::VBO = 0;
GLuint Lamp::EBO = 0;
GLuint Lamp::VAO = 0;

void Lamp::staticInit(const GLfloat *vertices, GLuint64 ver_size, const GLuint *indices, GLuint64 ind_size) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, ver_size, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_size, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)nullptr);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Lamp::draw(const Shader &shader, const glm::mat4 &view, const glm::mat4 &projection) const {
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shader.program, "lightColor"), color_.x, color_.y, color_.z);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Lamp::staticDeinit() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Lamp::set_pos(glm::vec3 &&position) {
    position_ = position;
    model_ = glm::translate(glm::mat4(1.0f), position);
}

void Lamp::use(const Shader &shader) const {
    glUniform3f(glGetUniformLocation(shader.program, "lightPos"), position_.x, position_.y, position_.z);
    glUniform4f(glGetUniformLocation(shader.program, "lightColor"), color_.r, color_.g, color_.b, color_.a);

//    glUniform3f(glGetUniformLocation(shader.program, lamp_uniform_.ambient.c_str()), color_.r / 5, color_.g / 5, color_.b / 5);
//    glUniform3f(glGetUniformLocation(shader.program, lamp_uniform_.diffuse.c_str()), color_.r / 2, color_.g / 2, color_.b / 2);
//    glUniform3f(glGetUniformLocation(shader.program, lamp_uniform_.specular.c_str()), color_.r / 1, color_.g / 1, color_.b / 1);
//
//    glUniform1f(glGetUniformLocation(shader.program, lamp_uniform_.constant.c_str()), 1.0f);
//    glUniform1f(glGetUniformLocation(shader.program, lamp_uniform_.linear.c_str()), 0.03f);
//    glUniform1f(glGetUniformLocation(shader.program, lamp_uniform_.quadratic.c_str()), 0.015f);
}

class DirLamp {
public:
    DirLamp(glm::vec3 dir, glm::vec3 clr, GLuint number) :
            direction_(dir),
            color_(clr),
            form_name_("dirLights[" + std::to_string(number) + "]") {
        lamp_uniform_.direction = form_name_ + ".direction";

        lamp_uniform_.ambient = form_name_ + ".ambient";
        lamp_uniform_.diffuse = form_name_ + ".diffuse";
        lamp_uniform_.specular = form_name_ + ".specular";
    };

    void use(const Shader &shader) const;

    [[maybe_unused]] void setDir(glm::vec3 &&direction) { direction_ = direction; }

    [[nodiscard]] glm::vec3 getDir() const { return direction_; }

private:
    struct {
        std::string direction;

        std::string ambient;
        std::string diffuse;
        std::string specular;
    } lamp_uniform_;

    glm::vec3 direction_;
    glm::vec3 color_;

    std::string form_name_;
};

void DirLamp::use(const Shader &shader) const {
    glUniform3f(glGetUniformLocation(shader.program, lamp_uniform_.direction.c_str()), direction_.x, direction_.y, direction_.z);

    glUniform3f(glGetUniformLocation(shader.program, lamp_uniform_.ambient.c_str()), color_.r / 5, color_.g / 5, color_.b / 5);
    glUniform3f(glGetUniformLocation(shader.program, lamp_uniform_.diffuse.c_str()), color_.r / 2, color_.g / 2, color_.b / 2);
    glUniform3f(glGetUniformLocation(shader.program, lamp_uniform_.specular.c_str()), color_.r / 1, color_.g / 1, color_.b / 1);
}


#endif //DRAW_LAMP_H
