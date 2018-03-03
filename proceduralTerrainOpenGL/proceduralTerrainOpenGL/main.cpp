#include "shader/shader_m.h"
#include "cameraZ.h"
#include "noise/FractalNoise.h"

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//settings
//laptop
int screenHeight = 768;
int screenWidth = 1366; 
//desktop
//int screenHeight = 1080;
//int screenWidth = 1960;
//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//map size 
const int baseVertsPerSection = 128;
float distancePerWorldSpace = 100.0f;
int lod=1;
//noise
int seed = 12345;
int octaves = 4;
float amplitude = 200.0f;
float wavelength = 2000.0f;
// camera
float cameraSpeed=100.0f;
float initialHeight = 50.0f;
float minViewDistance = 10.0f;
float maxViewDistance = 30000.0f;
Camera camera(glm::vec3(0.0f, 0.0f, initialHeight/distancePerWorldSpace),distancePerWorldSpace,cameraSpeed);
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
// function to resize viewport when user resizes window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//function to process key inputs
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
//function to process mouse actions
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = lastX-xpos;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	
	camera.ProcessMouseMovement(xoffset, yoffset);
}
//function to handle zooming
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
//function to create vertex data for a section of terrain with specified origin
float* createVertices(float originX, float originY, int baseVertsPerSection, float distancePerWorldSpace,int levelOfDetail, FractalNoise &f) {
	float lodFactor = pow(2, 1 - levelOfDetail);
	const int numVerts = int(pow(baseVertsPerSection*lodFactor,2));
	float* vertices = new float[numVerts * 3];
	for (int i = 0; i < numVerts * 3; i += 3) {//assign coordinates to each vertex 
		float x = ((i / 3) % int(baseVertsPerSection*lodFactor))- baseVertsPerSection*lodFactor /2.0f;
		float y = i / (baseVertsPerSection*lodFactor * 3)- baseVertsPerSection*lodFactor /2.0f;
		vertices[i] = float(x)/lodFactor;
		vertices[i + 1] = float(y)/lodFactor;
		float temp = f.noise(vertices[i]*distancePerWorldSpace + originX, vertices[i+1]*distancePerWorldSpace + originY, 0.0f);
		vertices[i + 2] = std::fmax(temp, 0.3*temp);
		//vertices[i + 2] = 0.5*temp*(0.3*exp(-8 * temp) + exp(8 * temp)) / (cosh(8 * temp));//Jungle
		//vertices[i + 2] = 0.5*(0.3*temp*exp(-2 * temp) + (0.3*temp+1)*exp(2 * temp)) / (cosh(2 * temp));//Mooreland
		//vertices[i + 2] = temp*tanh(temp);
		
	}
	return vertices;
}
unsigned int* createIndices(int levelOfDetail,int baseVertsPerSection) {
	float lodFactor = pow(2, 1 - levelOfDetail);
	int sectionLength = int(baseVertsPerSection*lodFactor);
	const int numIndices = 6 * pow(sectionLength - 1, 2);
	unsigned int* indices = new unsigned int[numIndices];
	int index = 0;
	for (int i = 0; i < numIndices; i += 6) {
		if (index%sectionLength == sectionLength - 1) { index++; }
		indices[i] = index;//first triangle
		indices[i + 1] = index + 1;
		indices[i + 2] = index + sectionLength;

		indices[i + 3] = index + 1;//second triangle
		indices[i + 4] = index + sectionLength;
		indices[i + 5] = index + sectionLength + 1;
		index++;

	}
	return indices;
}
int main() {
	//intialise GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//create GLFW window object
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "TerrainGen", glfwGetPrimaryMonitor(), nullptr);
	if (window == nullptr) {
		std::cerr << "Error: Failed to create window.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// register callback functions 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//initialise GLAD 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Error: Failed to initialise GLAD.\n";
		return -1;
	}
	//set viewport
	glViewport(0, 0, screenWidth,screenHeight);
	//set global gl settings
	glEnable(GL_DEPTH_TEST);
	//----------------------------------------------------------------
	Shader myShader("shader.vs.txt", "shader.fs.txt");
	//------------------------------------------------------
	//SET UP VERTEX DATA AND BUFFERS
	//---------------------------------------------------------
	int vertsPerSection = baseVertsPerSection* pow(2, 1 - lod);
	const int mapSize = pow(vertsPerSection,2);
	const int numIndices = 6 * pow(vertsPerSection - 1, 2);
	//vertices
	FractalNoise f(seed);
	f.setOctaves(octaves);
	f.setBaseAmplitude(amplitude / distancePerWorldSpace);
	f.setBaseFrequency(1.0/wavelength);
	
	float* vertices = createVertices(0.0f, 0.0f, baseVertsPerSection,distancePerWorldSpace,lod, f);
	unsigned int* indices = createIndices(lod,baseVertsPerSection);
	
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	
	
	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*mapSize*3, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*numIndices, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//LOAD AND CREATE TEXTURES
	//----------------------------------------------------
	unsigned int texture1, texture2;
	//texture 1(high alt)
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load("textures/mountain.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//texture 2(low alt)
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	//stbi_set_flip_vertically_on_load(true);
	data = stbi_load("textures/jungle1.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//---------------------------------------------------------------------
	//SET UP SHADER
	//----------------------------------------------------------------------
	myShader.use();
	myShader.setFloat("dpws", distancePerWorldSpace);
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//RENDER LOOP
	//------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		//update time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		processInput(window);
		
		glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		

		myShader.use();
		//set transformations
		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();
		int viewLoc = glGetUniformLocation(myShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//projection
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), float(screenWidth) / float(screenHeight), minViewDistance/distancePerWorldSpace, maxViewDistance/distancePerWorldSpace);
		int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//render terrain
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,numIndices, GL_UNSIGNED_INT, 0);
		/*for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime()*0.2f*(i+1), glm::vec3(1.0f, 0.3f, 0.5f));
			int modelLoc = glGetUniformLocation(myShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, 1350, GL_UNSIGNED_INT, 0);
			
		}*/
		

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete[] vertices;
	delete[] indices;
	glfwTerminate();
	return 0;
}
