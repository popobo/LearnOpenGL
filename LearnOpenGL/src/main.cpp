#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Practice.h"
#include "Shader.h"
#include "std_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include <filesystem>
#include "utils.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int POINT_LIGHT_NUMBER = 4;

glm::vec3 lightPos(1.2f, 0.0f, 4.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCREEN_WIDTH / 2;
float lastY = SCREEN_HEIGHT / 2;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

void processInput(GLFWwindow* window) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

unsigned int loadTexture(const char *path) {
	unsigned int textureID;
	GLCall(glGenTextures(1, &textureID));

	int width, height, nrComponents;

	stbi_set_flip_vertically_on_load(true);

	unsigned char * data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (!data){
		std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
		return textureID;
	}
	GLenum format;
	if (nrComponents == 1) {
		format = GL_RED;
	} else if (nrComponents == 3) {
		format = GL_RGB;
	} else if (nrComponents == 4) {
		format = GL_RGBA;
	}

	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	stbi_image_free(data);

	return textureID;
}


int main()
{
	bool praticing = false;
	if (praticing) {
		Practice practice;
		practice.Practice3_4();
		return 0;
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//告诉OpenGL渲染窗口的大小
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//监听窗口变化
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader shader("res/shaders/shader.vs", "res/shaders/shader.fs");
	Shader lightShader("res/shaders/lightShader.vs", "res/shaders/lightShader.fs");

	float vertices[] = {
        // positions          // normals           // texture coords
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


	
	unsigned int VAO;
	GLCall(glGenVertexArrays(1, &VAO));

	unsigned int VBO;
	GLCall(glGenBuffers(1, &VBO));

	//绑定VAO
	GLCall(glBindVertexArray(VAO));
	//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	//把之前定义的顶点数据复制到缓冲的内存中
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	//告诉OpenGL该如何解析顶点数据
	// 位置属性
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));

	// 漫反射贴图
	std::filesystem::path filePath1 = std::filesystem::absolute("res/textures/container2.png");
	unsigned int diffuseMap = loadTexture(filePath1.string().c_str());

	std::filesystem::path filePath2 = std::filesystem::absolute("res/textures/container2_specular.png");
	unsigned int specularMap = loadTexture(filePath2.string().c_str());

	std::filesystem::path filePath3 = std::filesystem::absolute("res/textures/matrix.jpg");
	unsigned int matrixMap = loadTexture(filePath3.string().c_str());

	unsigned int lightVAO;
	GLCall(glGenVertexArrays(1, &lightVAO));

	//绑定VAO
	GLCall(glBindVertexArray(lightVAO));
	//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	//把之前定义的顶点数据复制到缓冲的内存中
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	//告诉OpenGL该如何解析顶点数据
	// 位置属性
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	// 0是顶点属性的location
	GLCall(glEnableVertexAttribArray(0));

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

	GLCall(glEnable(GL_DEPTH_TEST));
	
	// 隐藏光标并捕捉它
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glm::vec3 pointLightPositions[POINT_LIGHT_NUMBER] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	while (!glfwWindowShouldClose(window)) {
		//检测输入
		processInput(window);
		
		//渲染指令
		//状态设置函数
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		//状态使用函数
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		shader.use();

		shader.setVec3("viewPos", camera.Position);
		// material properties
		shader.setFloat("material.shininess", 32.0f);

		//定向光
		shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		
		
		//点光源
		for (int i = 0; i < POINT_LIGHT_NUMBER; ++i) {
			std::string pointLightsI = "pointLights[";
			pointLightsI += std::to_string(i);
			pointLightsI += "].";
			shader.setVec3(pointLightsI + "position", pointLightPositions[i]);
			shader.setVec3(pointLightsI + "ambient", 0.05f, 0.05f, 0.05f);
			shader.setVec3(pointLightsI + "diffuse", 0.8f, 0.8f, 0.8f);
			shader.setVec3(pointLightsI + "specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat(pointLightsI + "constant", 1.0f);
			shader.setFloat(pointLightsI + "linear", 0.09f);
			shader.setFloat(pointLightsI + "quadratic", 0.032f);
		}

		//聚光
		shader.setVec3("spotLight.position", camera.Position);
		shader.setVec3("spotLight.direction", camera.Front);
		shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09f);
		shader.setFloat("spotLight.quadratic", 0.032f);
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		
		// view/projection transformations
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
		shader.setMatrix4("view", view);
		shader.setMatrix4("projection", projection);

		shader.setInt("material.diffuse", 0);
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, diffuseMap));

		shader.setInt("material.specular", 1);
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, specularMap));

		shader.setInt("material.emission", 2);
		GLCall(glActiveTexture(GL_TEXTURE2));
		GLCall(glBindTexture(GL_TEXTURE_2D, matrixMap));

		GLCall(glBindVertexArray(VAO));
		for (unsigned int i = 0; i < 10; ++i) {
			// world transformation
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMatrix4("model", model);

			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}

		lightShader.use();
		lightShader.setMatrix4("projection", projection);
		lightShader.setMatrix4("view", view);
		glBindVertexArray(lightVAO);
		for (int i = 0; i < POINT_LIGHT_NUMBER; ++i) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMatrix4("model", model);

			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}

		//检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	GLCall(glDeleteVertexArrays(1, &VAO));
	GLCall(glDeleteVertexArrays(1, &lightVAO));
	glfwTerminate();
	
	return 0;
}