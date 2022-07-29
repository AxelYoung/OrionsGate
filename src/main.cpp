#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>

#include <shader.h>
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader shader);

int main() {
	// Initialize GLFW
	glfwInit();
	// Configure GLFW options
	// Major version is 3 in 3.3 (ie 4 in 4.2)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// Minor version is 3 in 3.3 (ie 2 in 4.2)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Set OpenGL to use Core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create windowed mode window of width 800, height 600, and name Astrala
	GLFWwindow* window = glfwCreateWindow(800, 600, "Astrala", NULL, NULL);
	
	// Set main context to our window
	glfwMakeContextCurrent(window);
	// Callback to change window and viewport size on user change
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLAD
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Shader shader("default.vert", "default.frag");

	// Basic stuff, defining verticies each with 3 points (x, y, z), 3 verts make a triangle
	float verts[] {
		 0.2686499f, 0.5f, -0.4653155f,
		 0.5373f, 0.5f, 0.0f,
		 0.26865f, 0.5f, 0.4653155f,
		-0.26865f, 0.5f, 0.4653154f,
		-0.5373f, 0.5f, 0.0f,
		-0.2686499f, 0.5f, -0.46543155f,
		 0.2686499f, -0.5f, -0.4653155f,
		 0.5373f, -0.5f, 0.0f,
		 0.26865f, -0.5f, 0.4653155f,
		-0.26865f, -0.5f, 0.4653154f,
		-0.5373f, -0.5f, 0.0f,
		-0.2686499f, -0.5f, -0.46543155f,
	};

	unsigned int indices[]{
		0, 5, 1,
		5, 4, 1,
		4, 3, 1,
		3, 2, 1,
		6, 11, 7,
		11, 10, 7,
		10, 9, 7,
		9, 8, 7,
		0, 1, 6,
		6, 7, 1,
		1, 2, 7,
		7, 8, 2,
		2, 3, 8,
		8, 9, 3,
		3, 4, 9,
		9, 10, 4,
		4, 5, 10,
		10, 11, 5
	};

	// VBO = vertex buffer object, we are initialzing to protect that chunk of data at the given pointer
	// EBO = Element buffer objects, stores indices
	// VAO = vertex array object, stores pointer information for VBO attributes
	unsigned int VBO, EBO, VAO;

	// Generate one buffer object, 1 specifies amount, &VBO points to memory location
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Generate one vertex array object, store info in VAO pointer
	glGenVertexArrays(1, &VAO);

	// Binds the VAO so any VertexAttribPointer of EnableVertexAttrib will be stored in it
	glBindVertexArray(VAO);

	// Binds VBO to the type of GL_ARRAY_BUFFER, which is used for vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copies the verts array into the buffer memory location. GL_ARRAY_BUFFER is the type, 
	//sizeof(verts) is the amount of data in bytes, verts is the data, and fourth parameter determines how the GPU manages the given data
	// GL_STREAM_DRAW for data set once and used a few times, GL_STATIC_DRAW for data set once and used many times, and GL_DYNAMIC_DRAW is when it is set and used many times
	// What this actually changes is the location of the data in memory, depending on choice, will be placed in memory that allows faster or slower reads / writes
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 1st parameter specifies the location of the first vertex attribute (which we specified in vertex shader as 0)
	// 2nd parameter specifies the size of the vertex attribute, which is 3 as we used a vec3
	// 3rd parameter specifies the type of data used, which is a float
	// 4th parameter specifies if the data should be normalized???
	// 5th parameter is the "stride" or length in between each vertex attribute in bytes, so the size of 3 floats
	// 6th parameter is the initial offset of vertex attributes in the buffer (with a weird cast of void*)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the vertex attribute at a specific position in the buffer
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// Enable the vertex attribute at a specific position in the buffer
	//glEnableVertexAttribArray(1);

	// Sets polygon rendering mode to wireframe (first parameter is which side to apply to, 2nd is wireframe mode)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	unsigned int unpopped;
	glGenTextures(1, &unpopped);
	glBindTexture(GL_TEXTURE_2D, unpopped);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data { stbi_load("src/img/unpoppedcat.png", &width, &height, &nrChannels, 0) };
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << '\n';
	}
	stbi_image_free(data);

	unsigned int popped;
	glGenTextures(1, &popped);
	glBindTexture(GL_TEXTURE_2D, popped);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("src/img/poppedcat.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << '\n';
	}
	stbi_image_free(data);

	shader.Activate();

	glUniform1i(glGetUniformLocation(shader.ID, "unpoppedTexture"), 0);
	glUniform1i(glGetUniformLocation(shader.ID, "poppedTexture"), 1);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// While the window is open and not needing to be closed
	while (!glfwWindowShouldClose(window)) {

		processInput(window, shader);

		glClearColor(0.37f, 0.35f, 0.43f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, unpopped);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, popped);

		// Uses all the shaders in the shader program object, thus rendering an image
		shader.Activate();

		glm::mat4 view{ glm::mat4(1.0f) };
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
		shader.SetMatrix("view", view);

		glm::mat4 projection{ glm::mat4(1.0f) };
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		shader.SetMatrix("projection", projection);

		glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model{ glm::mat4(1.0f) };
			//model = glm::translate(model, cubePositions[i]);
			float angle { 250.0f * (float)glfwGetTime() };
			model = glm::rotate(model, glm::radians(angle), glm::vec3(.25f, 1.0f, 0.0f));
			shader.SetMatrix("model", model);
			// Draw an array of a particluar shape, with indice count of param 2, type of param 3, and offset of param 4
			glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);
		//}
		glBindVertexArray(0);

		// Outputs the array of colors (pixels) to the screen
		glfwSwapBuffers(window);
		// Checks for input (ie mouse / keyboard) and relays proper callbacks
		glfwPollEvents();
	}

	// Destroy all created objects
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	// Destroy the active window
	glfwDestroyWindow(window);
	// Terminate GLFW
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Shader shader) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}