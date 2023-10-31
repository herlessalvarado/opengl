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
    glm::vec3 initialMinExtents, currentMinExtents;
    glm::vec3 initialMaxExtents, currentMaxExtents;

    Object() {
        initialMinExtents = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        initialMaxExtents = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    };

    virtual GLuint setup()=0;
    virtual void display(Shader &sh)=0;

    void calculateBoundingBox(glm::mat4 model) {
        for (const glm::vec3& vertex : vertices) {
            initialMinExtents.x = std::fmin(initialMinExtents.x, vertex.x);
            initialMinExtents.y = std::fmin(initialMinExtents.y, vertex.y);
            initialMinExtents.z = std::fmin(initialMinExtents.z, vertex.z);
            initialMaxExtents.x = std::fmax(initialMaxExtents.x, vertex.x);
            initialMaxExtents.y = std::fmax(initialMaxExtents.y, vertex.y);
            initialMaxExtents.z = std::fmax(initialMaxExtents.z, vertex.z);
        }
        glm::vec4 minExtentsTransformed = model * glm::vec4(initialMinExtents, 1.0f);
        glm::vec4 maxExtentsTransformed = model * glm::vec4(initialMaxExtents, 1.0f);
        currentMinExtents = glm::vec3(minExtentsTransformed);
        currentMaxExtents = glm::vec3(maxExtentsTransformed);
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
