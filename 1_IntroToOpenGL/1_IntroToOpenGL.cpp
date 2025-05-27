#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"

//using namespace std;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 1024;

// time settings
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
// --------

// handling input
float displacement_X = 0.0f;
float displacement_Y = 0.0f;
float displacement_Z = 0.0f;
float movementSpeed = 1.0f;
// --------------

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // ------------------------------

    // glfw: window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "5SAE0PG003 2025 Lab 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    // ---------------------

    // glad: load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load OpenGL function pointers!" << std::endl;
        glfwTerminate();
        return -1;
    }
    // -----------------------------------

    // Shader loading and building
    Shader myFirstShader("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");

    // Geometry definition
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //left corner
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //right corner
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f //top corner
    };

    unsigned int VBO, VAO;

    /*int a = 10;
    int* b = &a;

    int c = *b + 5;

    *aPointer = 10;*/

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute for POSITION
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // -----------------------------

    // Vertex attribute for COLOR
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // --------------------------

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
        glClear(GL_COLOR_BUFFER_BIT);
        // -------------------------

        // enable shader and update uniform variables
        myFirstShader.use();
        myFirstShader.setVec3("displacement", displacement_X, displacement_Y, displacement_Z);
        // ------------------------------------------

        // RENDERING
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
        displacement_Y += movementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        displacement_Y -= movementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        displacement_X += movementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        displacement_X -= movementSpeed * deltaTime;
    }

    // Move in the Z axis
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        displacement_Z -= movementSpeed * deltaTime;
        std::cout << displacement_Z << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        displacement_Z += movementSpeed * deltaTime;
        std::cout << displacement_Z << std::endl;
    }
}

// frame buffer resizing callback
void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}