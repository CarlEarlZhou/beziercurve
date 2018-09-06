#ifndef MSHADER_H
#define MSHADER_H
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace glm;

class MShader {
public:
    unsigned int shaderProgram;
    MShader(string vertexShaderSourcePath, string fragmentShaderSourcePath) {
        shaderProgram = glCreateProgram();
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        string tem1 = readFile(vertexShaderSourcePath);
        const char* vertexShaderSource = tem1.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        string tem2 = readFile(fragmentShaderSourcePath);
        const char* fragmentShaderSource = tem2.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() {
        glUseProgram(shaderProgram);
    }

    void setMat4fv(char* name, mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, value_ptr(value));
    }

    void setVec3fv(char* name, vec3 value) {
        glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, value_ptr(value));
    }

    void setInt(char* name, int value) {
        glUniform1i(glGetUniformLocation(shaderProgram, name), value);
    }

    void setFloat(char* name, float value) {
        glUniform1f(glGetUniformLocation(shaderProgram, name), value);
    }
private:
    string readFile(string shaderSourcePath) {
        fstream shaderFile(shaderSourcePath);
        string shaderSource = "";
        string temStr = "";
        while (getline(shaderFile, temStr)) {
            shaderSource += temStr + "\n";
        }
        return shaderSource;
    }

};
#endif