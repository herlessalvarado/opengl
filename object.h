//
// Created by Herless Alvarado on 30/10/23.
//

#ifndef OPENGL_OBJECT_H
#define OPENGL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "shader.h"
#include "stb_image.h"

using namespace std;

class Object {
public:
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> textureCoords;
    GLuint VA0, VBO;

    Object() {};

    virtual GLuint setup()=0;
    virtual void display(Shader &sh)=0;
};

class Cube: public Object{
public:
    Cube() {};
    GLuint setup() override;
    void display(Shader &sh) override;
};


#endif //OPENGL_OBJECT_H
