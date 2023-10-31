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
    glm::vec3 initialPos;
    glm::vec3 minExtents;
    glm::vec3 maxExtents;

    Object() {
        minExtents = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        maxExtents = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    };

    virtual GLuint setup()=0;
    virtual void display(Shader &sh)=0;

    void calculateBoundingBox() {
        for (const glm::vec3& vertex : vertices) {
            minExtents.x = std::fmin(minExtents.x, vertex.x);
            minExtents.y = std::fmin(minExtents.y, vertex.y);
            minExtents.z = std::fmin(minExtents.z, vertex.z);
            maxExtents.x = std::fmax(maxExtents.x, vertex.x);
            maxExtents.y = std::fmax(maxExtents.y, vertex.y);
            maxExtents.z = std::fmax(maxExtents.z, vertex.z);
        }
    }
};

class Cube: public Object{
public:
    Cube(glm::vec3 pos) {
        initialPos = pos;
    };
    GLuint setup() override;
    void display(Shader &sh) override;
};


#endif //OPENGL_OBJECT_H
