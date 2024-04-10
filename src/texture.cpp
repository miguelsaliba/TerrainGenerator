#include "texture.h"
#include <stb_image.h>
#include <iostream>

TextureHandler::TextureHandler() {
    stbi_set_flip_vertically_on_load(true);
}

TextureHandler::~TextureHandler() {
    glDeleteTextures(texture_ids_.size(), texture_ids_.data());
}

void TextureHandler::loadTextures(const vector<string>& texture_filenames) {
    for (const auto& texture_filename : texture_filenames) {
        GLuint texture_id = loadTexture(texture_filename);
        if (texture_id != 0) {
            texture_ids_.push_back(texture_id);
        }
    }
}

void TextureHandler::bindTexture(GLenum texture_unit, GLuint texture_id) {
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

GLuint TextureHandler::loadTexture(const string& texture_filename) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texture_filename.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << texture_filename << std::endl;
        return 0;
    }

    GLenum format = GL_RGB;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return texture_id;
}

void TextureHandler::bindTextures() {
    // loop over all textures from GL_TEXTURE0 to GL_TEXTURE5
    for (size_t i = 0; i < texture_ids_.size(); i++) {
        bindTexture(GL_TEXTURE0 + i, texture_ids_[i]);
    }
}

