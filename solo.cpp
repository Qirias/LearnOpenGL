#define GL_GLEXT_PROTOTYPES
#include "includes/shader.h"
#include "Mesh.h"
#include "includes/camera.h"
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"
#include "includes/stb_image.h"
#include <filesystem>
namespace fs = std::filesystem;


const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
bool showFPS = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.5f, 0.2f, 3.5f);
unsigned int loadTexture(char const * path);

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glfwGetFramebufferSize(window, &width, &height);
    std::cout << "Width:\t" << width << "\t|\tHeight:\t" << height << "\n";
    glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    /*========Handling Controls========*/
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * 2.0);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * 2.0f);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * 2.0);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * 2.0f);
    /*=================================*/
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //Window close
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) //Lines of Triangles
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) //Fill the Triangles
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
        showFPS = !showFPS;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    //Retrieving the distance from last position and
    //save the current one to the last position variables
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;

    window = glfwCreateWindow(screenWidth, screenHeight, "Solo Project", NULL, NULL);

    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::cout << glGetString(GL_VERSION) << std::endl;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, //Βack Side
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //Front Side
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //Left Side
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //Right Side
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, //Bottom Side
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //Top Side
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};  

    ShaderProgramSource source = ParseShader("res/Basic.shader");
    ShaderProgramSource light  = ParseShader("res/LightSource.shader");

    unsigned int lampShader     = CreateShader(light.VertexShader, light.FragmentShader);
    unsigned int shader         = CreateShader(source.VertexShader, source.FragmentShader);

    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lampVao;
    glGenVertexArrays(1, &lampVao);
    glBindVertexArray(lampVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture(fs::path("res/container2.png").c_str());
    unsigned int specularMap = loadTexture(fs::path("res/container2_specular.png").c_str());
    
    useShader(shader);


    glEnable(GL_DEPTH_TEST);


    double lastTime = 0.0;
    unsigned int counter = 0;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        useShader(shader);
        setVec3(shader, "viewPos", camera.Position);

        // directional light
        setVec3(shader,"dirLight.direction", -0.2f, -1.0f, -0.3f);
        setVec3(shader,"dirLight.ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader,"dirLight.diffuse", 1.0f, 1.0f, 1.0f);
        setVec3(shader,"dirLight.specular", 1.0f, 1.0f, 1.0f);
        // point light 1
        setVec3(shader, "pointLights[0].position", pointLightPositions[0]);
        setVec3(shader, "pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader, "pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        setVec3(shader, "pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "pointLights[0].constant", 1.0f);
        setFloat(shader, "pointLights[0].linear", 0.09);
        setFloat(shader, "pointLights[0].quadratic", 0.032);
        // point light 2
        setVec3(shader, "pointLights[1].position", pointLightPositions[1]);
        setVec3(shader, "pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader, "pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        setVec3(shader, "pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "pointLights[1].constant", 1.0f);
        setFloat(shader, "pointLights[1].linear", 0.09);
        setFloat(shader, "pointLights[1].quadratic", 0.032);
        // point light 3
        setVec3(shader, "pointLights[2].position", pointLightPositions[2]);
        setVec3(shader, "pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader, "pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        setVec3(shader, "pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "pointLights[2].constant", 1.0f);
        setFloat(shader, "pointLights[2].linear", 0.09);
        setFloat(shader, "pointLights[2].quadratic", 0.032);
        // point light 4
        setVec3(shader, "pointLights[3].position", pointLightPositions[3]);
        setVec3(shader, "pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader, "pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        setVec3(shader, "pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "pointLights[3].constant", 1.0f);
        setFloat(shader, "pointLights[3].linear", 0.09);
        setFloat(shader, "pointLights[3].quadratic", 0.032);
        // spotLight
        setVec3(shader, "spotLight.position", camera.Position);
        setVec3(shader, "spotLight.direction", camera.Front);
        setVec3(shader, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
        setVec3(shader, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        setVec3(shader, "spotLight.specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "spotLight.constant", 1.0f);
        setFloat(shader, "spotLight.linear", 0.09);
        setFloat(shader, "spotLight.quadratic", 0.032);
        setFloat(shader, "spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        setFloat(shader, "spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        //Color reflected Uniforms

        setInt(shader, "material.diffuse", 0);
        setInt(shader, "material.specular", 1);
        setFloat(shader, "material.shininess", 32.0f);        

        glm::mat4 proj  = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth /(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view =  camera.GetViewMatrix();
        setMat4(shader, "projection", proj);
        setMat4(shader, "view", view);


        glm::mat4 model = glm::mat4(1.0f);
        setMat4(shader, "model", model);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(vao);
        for (unsigned int i = 0; i < 10; i++)
        {
            float angle;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            if (i == 0)
                angle = 20.0f * 3;
            else
                angle = 20.0f * i;   
            model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            setMat4(shader, "model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        useShader(lampShader);
        setMat4(lampShader, "projection", proj);
        setMat4(lampShader, "view", view);
        
        glBindVertexArray(lampVao);
        for (unsigned int i = 0; i < 4; i++)
         {
             model = glm::mat4(1.0f);
             model = glm::translate(model, pointLightPositions[i]);
             model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
             setMat4(lampShader,"model", model);
             glDrawArrays(GL_TRIANGLES, 0, 36);
         }

        
        ++counter;
        double currentTime = glfwGetTime();
        
        if (currentTime - lastTime >= 1.0)
        {
                if (showFPS)
                    std::cout << "FPS:\t" << counter << std::endl;
                ++lastTime;
                counter = 0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &lampVao);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}

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

//g++ solo.cpp -std=c++17 -lstdc++fs -lGL -lglfw