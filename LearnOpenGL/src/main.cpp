#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Practice.h"

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main()
{
	bool praticing = true;
	if (praticing) {
		Practice pratice;
		pratice.Pratice3();
		return 0;
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//����OpenGL��Ⱦ���ڵĴ�С
	glViewport(0, 0, 800, 600);
	//�������ڱ仯
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	//����������ɫ������
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//�Ѷ�����ɫ��Դ�븽�ӵ���ɫ�����󣬲��ұ�����
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//������ɫ���������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//����������
	glUseProgram(shaderProgram);

	//����ɫ���������ӵ���������Ժ󣬲�����Ҫ��ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};

	unsigned int indices[] = {
		// ע��������0��ʼ! 
		// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
		// �����������±��������ϳɾ���

		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//��VAO
	glBindVertexArray(VAO);
	//���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//����OpenGL����ν�����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window)) {
		//�������
		processInput(window);
		
		//��Ⱦָ��
		//״̬���ú���
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		//״̬ʹ�ú���
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//��鲢�����¼�����������
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	
	return 0;
}