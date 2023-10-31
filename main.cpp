#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "object.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightBulbPos(0.0f, 7.0f, 0.0f);

// obstacles
vector<Cube> obstacles;
vector<glm::vec3> obstaclesPositions = {
        glm::vec3(-7.0f, 0.0f, 0.0f),
        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::vec3(3.0f, 0.0f, 0.0f)
};
int numberOfObstacles = obstaclesPositions.size();

// models
glm::vec3 marioPos(-10.0f, 0.0f, 0.0f);
float movementSpeed = 0.05f; // Adjust the speed as needed

bool isJumping = false;
bool hasReachedMaxJumpHeight = false;
float maxJumpHeight = 1.0f;
float jumpVelocity = 0.1f;
float jumpCooldown = 1.0f;
float jumpCooldownTimer = 0.0f;

bool CheckCollision(const glm::vec3& minA, const glm::vec3& maxA, const glm::vec3& minB, const glm::vec3& maxB) {
    return (
            minA.x <= maxB.x &&
            maxA.x >= minB.x &&
            minA.y <= maxB.y &&
            maxA.y >= minB.y &&
            minA.z <= maxB.z &&
            maxA.z >= minB.z
    );
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("../lighting.vs", "../lighting.fs");
    Shader lightShader("../light.vs", "../light.fs");
    Shader modelsShader("../models.vs", "../models.fs");

    // setup obstacles
    for(int i=0; i<numberOfObstacles; i++){
        Cube cube(obstaclesPositions[i]);
        cube.setup();
        obstacles.push_back(cube);
    }

    Model mario("../resources/mario/mario.obj");
    Model fragileBox("../resources/fragile_box/fragile_box.obj");
    Model lightBulb("../resources/light_bulb/light_bulb.obj");

    glm::vec3 marioMinInitial = mario.minExtents;
    glm::vec3 marioMaxInitial = mario.maxExtents;
    glm::vec3 marioMin, marioMax;
    glm::vec3 obstacleMinInitial = fragileBox.minExtents;
    glm::vec3 obstacleMaxInitial = fragileBox.maxExtents;
    glm::vec3 obstacleMin, obstacleMax;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.position", lightBulbPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        lightingShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        lightingShader.setFloat("material.shininess", 64.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render obstacles
        for(int i=0; i<numberOfObstacles; i++){
            obstacles[i].display(lightingShader);
        }

        if (CheckCollision(marioMin, marioMax, obstacleMin, obstacleMax)) {
            marioPos = glm::vec3(-10.0f, 0.0f, 0.0f);
        }

        // render mario
        if (jumpCooldownTimer > 0.0f) {
            jumpCooldownTimer -= deltaTime;
        }

        if (isJumping) {
            if(hasReachedMaxJumpHeight){
                marioPos.y -= jumpVelocity;
            }else{
                marioPos.y += jumpVelocity;
                if (marioPos.y >= maxJumpHeight) {
                    hasReachedMaxJumpHeight = true;
                }
            }
            if (marioPos.y <= 0.0f) {
                isJumping = false;
                hasReachedMaxJumpHeight = false;
                marioPos.y = 0.0f;
            }
        }


        modelsShader.use();
        modelsShader.setMat4("projection", projection);
        modelsShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, marioPos);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        float rotationAngle = glm::radians(180.0f); // Convert degrees to radians
        glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f); // Rotate around the y-axi
        model = glm::rotate(model, rotationAngle, rotationAxis);
        modelsShader.setMat4("model", model);

        // update bounding box
        model = glm::mat4(1.0f);
        model = glm::translate(model, marioPos);
        glm::vec4 marioMinTransformed = model * glm::vec4(marioMinInitial, 1.0f);
        glm::vec4 marioMaxTransformed = model * glm::vec4(marioMaxInitial, 1.0f);
        marioMin = glm::vec3(marioMinTransformed);
        marioMax = glm::vec3(marioMaxTransformed);

        mario.Draw(modelsShader);

        // render obstacle
        model = glm::mat4(1.0f);  // Reset the model matrix
        glm::vec3 obstaclePos(5.0f, 0.0f, 0.0f);  // Set obstacle position
        model = glm::translate(model, obstaclePos);
        modelsShader.setMat4("model", model);

        // update bounding box
        model = glm::mat4(1.0f);
        model = glm::translate(model, obstaclePos);
        glm::vec4 obstacleMinTransformed = model * glm::vec4(obstacleMinInitial, 1.0f);
        glm::vec4 obstacleMaxTransformed = model * glm::vec4(obstacleMaxInitial, 1.0f);
        obstacleMin = glm::vec3(obstacleMinTransformed);
        obstacleMax = glm::vec3(obstacleMaxTransformed);

        fragileBox.Draw(modelsShader);

        // lightBulb
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightBulbPos);
        model = glm::scale(model, glm::vec3(0.5f));
        rotationAngle = glm::radians(180.0f);
        rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        model = glm::rotate(model, rotationAngle, rotationAxis);
        lightShader.setMat4("model", model);
        lightBulb.Draw(lightShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    for(int i=0; i<numberOfObstacles; i++){
        glDeleteVertexArrays(1, &obstacles[i].VA0);
        glDeleteBuffers(1, &obstacles[i].VBO);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        marioPos.x -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        marioPos.x += movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!isJumping && jumpCooldownTimer <= 0.0f) {
            isJumping = true;
            jumpCooldownTimer = jumpCooldown;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}