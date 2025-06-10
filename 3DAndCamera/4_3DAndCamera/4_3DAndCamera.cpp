#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <stb_image.h>

#include "Shader.h"
#include "Camera.h"

//using namespace std;

float randomFloat()
{
    return (float)(rand()) / (float)(RAND_MAX);
}

// Callbacks
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
// ---------

// function declarations
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path, bool flipVertically);
// ---------------------

// settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 1024;

// time settings
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
// --------

// Mouse position data
float previousX = 0.0f;
float previousY = 0.0f;
bool isFirstFrame = true;
// -------------------

// handling input
//glm::vec3 displacement = glm::vec3(0.0f, 0.0f, -2.0f);
//float movementSpeed = 1.0f;
// --------------

// Camera declaration
Camera myCamera1(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f));
// ------------------
int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // ------------------------------

    // glfw: window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "5SAE0PG003 2025 Lab 2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    // ---------------------

    // Cursor grabbing settings
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // ------------------------

    // glad: load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load OpenGL function pointers!" << std::endl;
        glfwTerminate();
        return -1;
    }
    // -----------------------------------

    //Enabling-Disabling depth testing
    glEnable(GL_DEPTH_TEST);
    //--------------------------------

    // Shader loading and building
    Shader myFirstShader("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");

    // Geometry definition
    float vertices[] = {
        //positions		     //texture coords
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute for POSITION
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // -----------------------------

    // Vertex attribute for TEXTURE
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // --------------------------

    // Texture loading
    unsigned int texturePNG = loadTexture("Textures/awesomeface.png", true);
    unsigned int textureJPG = loadTexture("Textures/RTS_Crate.jpg", true);
    // ---------------

    // Unbinding VAO and VBO. This is not mandatory, but it will make the pipeline clearer.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // ----------------------

    // -------------------

    // MAIN RENDERING LOOP
    while (!glfwWindowShouldClose(window))
    {
        // DeltaTime calculation
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        // ---------------------

        // handle user input
        processInput(window);
        // -----------------

        // RENDER STUFF

        // Reinitialize frame buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // -------------------------

        // enable shader and update uniform variables
        myFirstShader.use();

        // declare transforms
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        // ------------------

        // calculate projection matrix
        // attributes: fov, aspect ratio, near clipping plane, far clipping plane
        projection = glm::perspective(glm::radians(myCamera1.fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        // ---------------------------

        // Update view matrix
        view = myCamera1.GetViewMatrix();
        // ------------------
        
        // model = identity
        // model = glm::translate(model, cubePositions[3] + displacement);
        // model = identity * translate = translate
        // model = glm::rotate(model, cosf(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = identity * translate * rotate = translate AND rotate

        // myFirstShader.setVec3("displacement", displacement);
        // myFirstShader.setMat4("model", model);
        myFirstShader.setMat4("view", view);
        myFirstShader.setMat4("projection", projection);
        // ------------------------------------------

        // Set texture samplers
        myFirstShader.setInt("textureObj[0]", 0);
        myFirstShader.setInt("textureObj[1]", 1);
        // --------------------

        // RENDERING
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureJPG);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texturePNG);

        for (int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);

            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

            myFirstShader.setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // ---------

        // ------------

        // glfw: double buffering and polling IO events (keyboard, mouse, etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
        // --------------------------------------------------------------------
    }
    // ------------------

    // Resource and GLFW cleanup
    // TODO resources
    glfwTerminate();
    // -------------------------

    return 0;
}

//Handle keyboard input events
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        myCamera1.HandleKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        myCamera1.HandleKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        myCamera1.HandleKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        myCamera1.HandleKeyboard(LEFT, deltaTime);
    }
    
    // Move up or down
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        myCamera1.HandleKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        myCamera1.HandleKeyboard(DOWN, deltaTime);
    }
}

// frame buffer resizing callback
void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}

// Mouse position callback
void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (isFirstFrame)
    {
        previousX = xPos;
        previousY = yPos;
        isFirstFrame = false;
    }

    float xOffset = xPos - previousX;
    float yOffset = yPos - previousY;

    previousX = xPos;
    previousY = yPos;

    myCamera1.HandleMouseMovement(xOffset, yOffset, deltaTime);
}

// Scrolling callback
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    myCamera1.HandleMouseScroll((float)yOffset);
}

unsigned int loadTexture(const char* path, bool flipVertically)
{
    unsigned int textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Texture wrapping properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // ---------------------------

    // Texture filtering properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // ----------------------------

    stbi_set_flip_vertically_on_load(flipVertically);

    int width, height, numOfChannels;
    unsigned char* data = stbi_load(path, &width, &height, &numOfChannels, 0);

    if (data)
    {
        GLenum format;

        if (numOfChannels == 1)
        {
            format = GL_RED;
        }
        else if (numOfChannels == 3)
        {
            format = GL_RGB;
        }
        else if (numOfChannels == 4)
        {
            format = GL_RGBA;
        }
        else
        {
            std::cout << "TEXTURE FILE FAILED TO LOAD: INCOMPATIBLE NUMBER OF CHANNELS!!" << std::endl;
            stbi_image_free(data);
            return textureID;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "TEXTURE FILE FAILED TO LOAD FROM PATH " << path << "!!" << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}