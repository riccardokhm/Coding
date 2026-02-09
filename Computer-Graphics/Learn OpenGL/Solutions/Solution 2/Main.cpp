#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "Shader.h"	

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"
#include "Main.h"

using namespace std;
using namespace glm;

#pragma region Fields

unsigned int VBO, VAO, EBO, vertexShader, fragmentShader,fragmentShader1, shaderProgram, shaderProgram1, texture1, texture2;

int vertexShadsuccess, fragmentShadsucces, fragmentShadsuccess, linkShadsuccess;
int width, height, nrChannels;
char infoLog[512];
bool isWireframe = false;

// Usa std::string per concatenare percorsi
string solutionDir = _SOLUTIONDIR;
string filePathTexture1 = solutionDir + "Floor.jpg";
string filePathTexture2 = solutionDir + "Wall.jpg";


float vertices[] = {

	//Position         //Color           // Texture Coords
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// bottom right
	 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top right
	 -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // center
	 -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // bottom left
};

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,1.0f,0.0f,
	0.5f, 0.5f, -0.5f,1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 
	-0.5f, 0.5f,-0.5f, 0.0f, 1.0f,
	-0.5f,-0.5f, -0.5f, 0.0f,0.0f,

	-0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 
	0.5f,-0.5f, 0.5f, 1.0f,0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,0.0f, 0.0f,

	-0.5f,0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f,0.5f, -0.5f, 1.0f,1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
	0.5f, -0.5f, 0.5f,0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f,0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 2, // first triangle
	2, 3, 0  // second triangle
};

#pragma endregion


#pragma region Methods

void transformation1()
{
	vec4 vec = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mat4 trans = mat4(1.0f);
	trans = translate(trans, vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;

	cout << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << endl;
}

mat4 scaleRotate() 
{
	mat4 trans = mat4(1.0f);
	trans = rotate(trans, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));	
	trans = scale(trans, vec3(0.5f, 0.5f, 0.5f));	
	return trans;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Wireframe mode
			isWireframe = true;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Fill mode
			isWireframe = false;
		}
	}
		
}

#pragma endregion


#pragma region Program

int main() 
{
	// Initialize GLFW and GLAD
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD " << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Setting vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Setting vertex attributes pointers for color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Setting vertex attributes pointers for texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader ourShader("Vertex.vert", "Fragment.frag");

	// Load and create a texture
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(filePathTexture1.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded successfully" << std::endl;
		stbi_image_free(data);
	}
	else 
	{
		std::cout << stbi_failure_reason() << std::endl;
		std::cout << "Failed to load texture" << std::endl;
	}

	// Load and create  texture2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load(filePathTexture2.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded successfully" << std::endl;
		stbi_image_free(data);
	}
	else
	{
		std::cout << stbi_failure_reason() << std::endl;
		std::cout << "Failed to load texture" << std::endl;
	}

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);	
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);

	// Apply initial transformations
	//transformation1();
	//mat4 trans = scaleRotate();

	// Move backwards elements in scene to render from distance
	mat4 model = mat4(1.0f);
	model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));

	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));

	mat4 projection;
	projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		// Create a time-based rotation transformation
		/*mat4 rot = mat4(1.0f);
		rot = rotate(rot, (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
		rot = translate(rot, vec3(0.5f, -0.5f, 0.0f));

		float scaleFactor = abs(sin(glfwGetTime()));
		mat4 scale = mat4(1.0f);
		scale = glm::scale(scale, vec3(scaleFactor, scaleFactor, 1.0f));*/

		model = rotate(model, (float)glfwGetTime() * radians(50.0f), vec3(0.5f, 1.0f, 0.0f));



		//events
		processInput(window);

		// Update the uniform green color
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

		int projectiveLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectiveLoc, 1, GL_FALSE, value_ptr(projection));


		//unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rot));

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//drawing commands
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the triangles using the indices

		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(scale));
		glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0); // Draw the outline using the indices

		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//  de allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

#pragma endregion

