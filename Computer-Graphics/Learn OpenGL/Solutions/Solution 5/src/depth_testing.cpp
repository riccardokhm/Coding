#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#include <Camera.h>
#include <Model.h>

#include <iostream>
#include <string>
#include <map>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path, bool isBlended);

string solutionDir = _SOLUTIONDIR;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LESS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // build and compile shaders
    // -------------------------
	string vertexShaderPath = solutionDir + "\\resources\\shaders\\depth_testing.vs";
	string fragmentShaderPath = solutionDir + "\\resources\\shaders\\depth_testing.fs";
	string outlineFragmentShaderPath = solutionDir + "\\resources\\shaders\\FragmentSingle.fs";
    Shader shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	Shader outlineShader(vertexShaderPath.c_str(), outlineFragmentShaderPath.c_str());


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
        // front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
        // left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         // bottom face          
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
         // top face
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
    };

    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
    };

    float quadVertices[] = {
        // positions in NDC space   // texture Coords
       -1.0f,  1.0f,   0.0f, 1.0f,
       -1.0f, -1.0f,   0.0f, 0.0f,
        1.0f, -1.0f,   1.0f, 0.0f,
       -1.0f,  1.0f,   0.0f, 1.0f,
        1.0f, -1.0f,   1.0f, 0.0f,
        1.0f,  1.0f,   1.0f, 1.0f								
	};

    vector<glm::vec3> positions;
    positions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    positions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    positions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    positions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    positions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	// creating a further FBO, a texture and a renderbuffer object to test the framebuffer completeness
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int texColorbuffer;
    glGenTextures(1, &texColorbuffer);
    glBindTexture(GL_TEXTURE_2D, texColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorbuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);  
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

	// vegetation VAO
    unsigned int vegetationVAO, vegetationVBO;
    glGenVertexArrays(1, &vegetationVAO);
    glGenBuffers(1, &vegetationVBO);
    glBindVertexArray(vegetationVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vegetationVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // windows VAO
    unsigned int windowVAO, windowVBO;
    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);
    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
	string cubeTexturePath = solutionDir + "resources\\img\\marble.jpg";
	string floorTexturePath = solutionDir +  "resources\\img\\metal.png";
	string grassTexturePath = solutionDir +  "resources\\img\\grass.png";
	string windowTexturePath = solutionDir + "resources\\img\\window.png";
    unsigned int cubeTexture  = loadTexture(cubeTexturePath.c_str(), false);
    unsigned int floorTexture = loadTexture(floorTexturePath.c_str(), false);
	unsigned int grassTexture = loadTexture(grassTexturePath.c_str(), true);
	unsigned int windowTexture = loadTexture(windowTexturePath.c_str(), true);

    // shader configuration
    // --------------------
    shader.use();

    // render loop
    // -----------
    while(!glfwWindowShouldClose(window))
    {

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		glStencilMask(0x00); // all bits are not writeable

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);


        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setInt("texture1", 0);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //glStencilMask(0xFF);

        // grass
        glDisable(GL_CULL_FACE);
        glBindVertexArray(vegetationVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture); 	
        shader.setInt("texture1", 0);
        for (size_t i = 0; i < positions.size() / 2; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // cubes
        glEnable(GL_CULL_FACE);
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        shader.setInt("texture1", 0);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
		// windows
        glDisable(GL_CULL_FACE);
        std::map<float, vec3>sorted;
		glBindVertexArray(windowVAO);
		glBindTexture(GL_TEXTURE_2D, windowTexture);
		shader.setInt("texture1", 0);
		for (size_t i = positions.size() / 2; i < positions.size(); i++)
		{
			float distance = glm::length(camera.Position - positions[i]);
			sorted[distance] = positions[i];
		}
		map<float, vec3>::reverse_iterator sortedIt = sorted.rbegin();
        for (sortedIt = sorted.rbegin() ; sortedIt != sorted.rend(); sortedIt++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, sortedIt ->second);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glEnable(GL_CULL_FACE);


        #pragma region Stencil_Test

        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilMask(0x00);
        //glDisable(GL_DEPTH_TEST);

  //      //scaled cubes
        //outlineShader.use();
  //      model = glm::mat4(1.0f);
  //      view = camera.GetViewMatrix();
  //      projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  //      outlineShader.setMat4("view", view);
  //      outlineShader.setMat4("projection", projection);
  //      model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        //model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
  //      outlineShader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
  //      model = glm::mat4(1.0f);
  //      model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
  //      outlineShader.setMat4("model", model);
  //      glDrawArrays(GL_TRIANGLES, 0, 36);
  // 
        //glStencilMask(0xFF);
        //glStencilFunc(GL_ALWAYS, 1, 0xFF);

        //glEnable(GL_DEPTH_TEST);    


        #pragma endregion


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

#pragma region Utility_Functions
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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path, bool isBlended)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    if (isBlended)
    {
        stbi_set_flip_vertically_on_load(true);
    }
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

        if(!isBlended)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

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

#pragma endregion
