#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

class TextureHandler {
public:
    TextureHandler();
    ~TextureHandler();
    void loadTextures(const vector<string>& texture_filenames);
    void bindTexture(GLenum texture_unit, GLuint texture_id);
    void bindTextures();
private:
    GLuint loadTexture(const string& texture_filename);
    vector<GLuint> texture_ids_;
};

#endif

