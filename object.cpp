//
// Created by Herless Alvarado on 30/10/23.
//

#include "object.h"

unsigned int loadTexture(const char *path);

const float PI = 3.14159265359;

GLuint Tunnel::setup() {
    float tunnelPoints[numCircles * numPoints * 3];

    for (int i = 0; i < numCircles; ++i) {
        for (int j = 0; j < numPoints; ++j) {
            float angle = 2.0f * PI * static_cast<float>(j) / static_cast<float>(numPoints);
            float x = circleRadius * cos(angle);
            float y = circleRadius * sin(angle);
            float z = static_cast<float>(i) * 0.2f;
            tunnelPoints[(i * numPoints + j) * 3] = x;
            tunnelPoints[(i * numPoints + j) * 3 + 1] = y;
            tunnelPoints[(i * numPoints + j) * 3 + 2] = z;
            normals.emplace_back(glm::vec3(1));
        }
    }

    for (int i = 0; i < numCircles - 1; ++i) {
        for (int j = 0; j < numPoints; ++j) {
            int currentIdx = i * numPoints + j;
            int nextIdx = (i + 1) * numPoints + j;

            if(j == numPoints - 1) {
                // first triangle
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3]);
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 2]);

                triangleVertices.emplace_back(tunnelPoints[(currentIdx - j) * 3 ]);
                triangleVertices.emplace_back(tunnelPoints[(currentIdx - j) * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[(currentIdx - j) * 3 + 2]);

                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 2]);

                // second triangle
                triangleVertices.emplace_back(tunnelPoints[(currentIdx - j) * 3 ]);
                triangleVertices.emplace_back(tunnelPoints[(currentIdx - j) * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[(currentIdx - j) * 3 + 2]);

                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 2]);

                triangleVertices.emplace_back(tunnelPoints[(nextIdx - j) * 3]);
                triangleVertices.emplace_back(tunnelPoints[(nextIdx - j) * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[(nextIdx - j) * 3 + 2]);
            } else {
                // first triangle
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3]);
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 2]);

                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 3]);
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 4]);
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 5]);

                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 2]);

                // second triangle
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 1]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 2]);

                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 3]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 4]);
                triangleVertices.emplace_back(tunnelPoints[nextIdx * 3 + 5]);

                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 3]);
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 4]);
                triangleVertices.emplace_back(tunnelPoints[currentIdx * 3 + 5]);
            }

            for (int k = 0; k < 3; ++k) {
                // first triangle
                triangleNormals.push_back(1);
                triangleNormals.push_back(1);
                triangleNormals.push_back(1);

                // second triangle
                triangleNormals.push_back(1);
                triangleNormals.push_back(1);
                triangleNormals.push_back(1);
            }
        }
    }

    glGenVertexArrays( 1, &VA0);
    glBindVertexArray( VA0);

    GLuint vbos[2];
    glGenBuffers( 2, vbos );

    glBindBuffer( GL_ARRAY_BUFFER, vbos[0] );
//    glBufferData( GL_ARRAY_BUFFER, sizeof(tunnelPoints), tunnelPoints, GL_STATIC_DRAW );
    glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(float), triangleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray( 0 );


    glBindBuffer( GL_ARRAY_BUFFER, vbos[1] );
//    glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW );
    glBufferData(GL_ARRAY_BUFFER, triangleNormals.size() * sizeof(float), triangleNormals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0 );
    glEnableVertexAttribArray( 1 );

    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    return VA0;
}
void Tunnel::display(Shader &sh) {
    glm::mat4 model = glm::mat4(1.0);
    sh.setMat4("model", model);
    glBindVertexArray(VA0);
//    glDrawArrays(GL_POINTS, 0, numCircles * numPoints);
    glDrawArrays(GL_TRIANGLES, 0, triangleVertices.size()/3);
    glBindVertexArray(0);
}

GLuint Curve::setup() {
    // x = y * cos(4*y)   y E [0,2pi]
    for(float y=0; y < 2*3.1415; y += 0.1) {
        float x = y * cos(4 * y);
        vertices.emplace_back(glm::vec3(x,y,0));
        normals.emplace_back(glm::vec3(1, sin(4*y),0));
    }
    glGenVertexArrays( 1, &VA0);
    glBindVertexArray( VA0);
    GLuint vbos[2];
    glGenBuffers( 2, vbos );

    glBindBuffer( GL_ARRAY_BUFFER, vbos[0] );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, vbos[1] );
    glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0 );
    glEnableVertexAttribArray( 1 );

    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    return VA0;
}
void Curve::display(Shader &sh) {
    glm::mat4 model = glm::mat4(1.0);
    sh.setMat4("model", model);
    glBindVertexArray(VA0);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
    glBindVertexArray(0);
}

GLuint Surface::setup() {
    // x = (2 + cos(t))*cos(phi)
    // y = (2 + cos(t))*sen(phi)
    // z = t
    float x, y, z;
    glm::vec3 dt, ds;
    for(float t=0; t < 2*3.1415; t += 0.1) {
        for(float phi=0; phi < 2*3.1415; phi += 0.1){
            x = (2 + cos(t))*cos(phi);
            y = (2 + cos(t))*sin(phi);
            z = t;
            dt = glm::vec3(-sin(t)*cos(phi), -sin(t)*sin(phi),1);
            ds = glm::vec3();
            vertices.emplace_back(glm::vec3(x,y,z));
            normals.emplace_back(glm::vec3(1));
        }
    }
    glGenVertexArrays( 1, &VA0 );
    glBindVertexArray( VA0 );
    GLuint vbos[2];
    glGenBuffers( 2, vbos );

    glBindBuffer( GL_ARRAY_BUFFER, vbos[0] );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, vbos[1] );
    glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0 );
    glEnableVertexAttribArray( 1 );

    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    return VA0;
}
void Surface::display(Shader &sh){
    glm::mat4 model = glm::mat4(1.0);
    sh.setMat4("model", model);
    glBindVertexArray(VA0);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
    glBindVertexArray(0);
}

GLuint Cube::setup(){
    float cubeVertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    int quantity =sizeof(vertices)/sizeof(float);
    for(int i=0; i < quantity; i+=8) {
        glm::vec3 v(cubeVertices[i], cubeVertices[i+1], cubeVertices[i+2]);
        glm::vec3 v2(cubeVertices[i+3], cubeVertices[i+4], cubeVertices[i+5]);
        glm::vec2 v3(cubeVertices[i+6], cubeVertices[i+7]);
        vertices.push_back(v);
        normals.push_back(v2);
        textureCoords.push_back(v3);
    }

    glGenVertexArrays(1, &VA0);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(VA0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return VA0;
}
void Cube::display(Shader &sh){
    unsigned int diffuseMap = loadTexture("../resources/textures/container.png");
    unsigned int specularMap = loadTexture("../resources/textures/container_specular.png");
    sh.setInt("material.diffuse", 0);
    sh.setInt("material.specular", 1);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, initialPos);
    // get bounding box
    calculateBoundingBox(model);
    sh.setMat4("model", model);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // render the cube
    glBindVertexArray(VA0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}