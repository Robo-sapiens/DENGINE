
#ifndef DRAW_TEXTURE_H
#define DRAW_TEXTURE_H

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <filesystem>

class Texture {
public:
    explicit Texture(const std::filesystem::path &diffuse_path,
                     const std::filesystem::path &normal_path,
                     const std::filesystem::path &parallax_path) {
        int width = 0;
        int height = 0;

        // Diffuse
        auto image = SOIL_load_image(
                diffuse_path.string().c_str(), &width, &height, nullptr, SOIL_LOAD_RGB
        );
        glGenTextures(1, &diffuse_tex_);
        glBindTexture(GL_TEXTURE_2D, diffuse_tex_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);

        // Normal
        image = SOIL_load_image(
                normal_path.string().c_str(), &width, &height, nullptr, SOIL_LOAD_RGB
        );
        glGenTextures(1, &normal_tex_);
        glBindTexture(GL_TEXTURE_2D, normal_tex_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);

        // Parallax
        image = SOIL_load_image(
                parallax_path.string().c_str(), &width, &height, nullptr, SOIL_LOAD_RGB
        );
        glGenTextures(1, &parallax_tex_);
        glBindTexture(GL_TEXTURE_2D, parallax_tex_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
    }

    void use(const Shader &shader) const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_tex_);
        glUniform1i(glGetUniformLocation(shader.program, diffuse_), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normal_tex_);
        glUniform1i(glGetUniformLocation(shader.program, specular_), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, parallax_tex_);
        glUniform1i(glGetUniformLocation(shader.program, parallax_), 2);

        glActiveTexture(GL_TEXTURE0);
    }

private:
    static constexpr char diffuse_[] = "diffuseMap";
    static constexpr char specular_[] = "normalMap";
    static constexpr char parallax_[] = "depthMap";

    unsigned int diffuse_tex_ = 0;
    unsigned int normal_tex_ = 0;
    unsigned int parallax_tex_ = 0;
};


#endif //DRAW_TEXTURE_H
