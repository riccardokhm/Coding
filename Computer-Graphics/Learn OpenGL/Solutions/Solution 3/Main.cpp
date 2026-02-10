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

float deltaTime, lastFrame;
float pitch, yaw;

// Set camera position
vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		// toggle wireframe / fill
		if (!isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
			isWireframe = true;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill mode
			isWireframe = false;
		}
	}

	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD " << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// Callbacks for input or user interaction with the current window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// IMPORTANT: cubeVertices layout is (position: vec3) + (texcoord: vec2) -> stride = 5 floats
	constexpr GLsizei stride = 5 * sizeof(float);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute (no color in cubeVertices)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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


	vec3 cubePosition[] = {
		vec3(0.0f, 0.0f, 0.0f),
		vec3(2.0f, 5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.2f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f, -0.4f, -3.5f),
		vec3(-1.7f, 3.0f, -7.5f),
		vec3(1.3f, -2.0f,-2.5f),
		vec3(1.5f, 2.0f, -2.5f),
		vec3(1.5f, 0.2f, -2.5f),
		vec3(-1.3f, 1.0f, -1.5f)
	};

	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));

	mat4 projection;
	projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// Activating depth testing
	glEnable(GL_DEPTH_TEST);

	const float radius = 10.0f;

	// Setting initial value of the camera
	yaw = 90.0f;
	pitch = 0.0f; 

	vec3 direction;
	direction.x = cos(radians(yaw)) * cos(radians(pitch));
	direction.y = sin(radians(pitch));
	direction.z = sin(radians(yaw)) * cos(radians(pitch));


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        //events
        processInput(window);

        // Recompute view matrix from camera position updated by WASD
        view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// Update the uniform green color
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//drawing commands
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			mat4 model = mat4(1.0f);

			model = translate(model, cubePosition[i]);
			float angle = 20.0f * i;
			model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));

			int modelLoc = glGetUniformLocation(ourShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			int viewLoc = glGetUniformLocation(ourShader.ID, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

			int projectiveLoc = glGetUniformLocation(ourShader.ID, "projection");
			glUniformMatrix4fv(projectiveLoc, 1, GL_FALSE, value_ptr(projection));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//  de allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(ourShader.ID);

	glfwTerminate();
	return 0;
}

#pragma endregion

