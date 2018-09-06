#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "mShader.h"
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define V_SIZE 700
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

float vertices[V_SIZE * 3];
double cn[50];
double ano[50];

float mouseX = 0, mouseY = 0;
vector<vec2> points;
bool points_change = false;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "caracal", NULL, NULL);
	if (window == NULL) {
		printf("err create window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("err load glad\n");
		return -1;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	MShader mshader("./test.vert", "./test.frag");

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		if (points_change) {
			for (int i = 0; i < V_SIZE * 3; ++i) {
				vertices[i] = 0.0f;
			}
			int vec_size = points.size();
			cn[0] = 1;
			for (int i = 1; i < vec_size; ++i) {
				cn[i] = (double)(vec_size - i)/i * cn[i - 1];
			}
			
			for (int i = 0; i < V_SIZE; ++i) {
				int index = i * 3;
				float u = (float)i / V_SIZE;
				double tem_a = 1;
				double tem_b = pow(1 - u, vec_size - 1);
				for (int j = 0; j < vec_size; ++j) {
					ano[j] = cn[j] * tem_a * tem_b;
					tem_a = tem_a * u;
					tem_b /= 1 - u;
				}
				
				for (int j = 0; j < vec_size; ++j) {
					vertices[index] = vertices[index] + points[j].x * ano[j];
					vertices[index + 1] = vertices[index + 1] + points[j].y *ano[j];
				}
			}
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			points.clear();
			printf("%d\n", points.size());
			points_change = false;
		}

		mshader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1200);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) switch(button){
		case GLFW_MOUSE_BUTTON_LEFT:
			points.push_back(vec2(mouseX/400-1, 1-mouseY/300));
			printf("%f %f\n", mouseX/400-1, 1-mouseY/300);
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			points_change = true;
			break;
		default:
			return;
	}
	return;
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}