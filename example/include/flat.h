
#ifndef DRAW_FLAT_H
#define DRAW_FLAT_H

#include <GL/glew.h>
#include "texture.h"

class Flat {
public:
    explicit Flat(Texture &texture) {

    }

    static void staticInit(const GLfloat *vertices, GLuint64 ver_size, const GLuint *indices, GLuint64 ind_size);

    static void staticDeinit();

private:
    static unsigned int VBO;
    static unsigned int EBO;
    static unsigned int VAO;
};

void Flat::staticInit(const GLfloat *vertices, GLuint64 ver_size, const GLuint *indices, GLuint64 ind_size) {
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
}

void Flat::staticDeinit() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

#endif //DRAW_FLAT_H
