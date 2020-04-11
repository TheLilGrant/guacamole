#ifndef _SHADER_PROGRAM_H
#define _SHADER_PROGRAM_H

#include <iostream>
#include <string>
#include <fstream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

class ShaderProgram
{
    public:
    unsigned int id;

    /* Read in a file as a string. Used for fetching shader source code */
    std::string readFile(const char *filePath)
    {
        std::string content;
        std::ifstream fileStream(filePath, std::ios::in);

        if (!fileStream.is_open()) {
            std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
            return "";
        }

        std::string line = "";
        while (!fileStream.eof()) {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }

        fileStream.close();
        return content;
    }

    /* Compiles and Links Shaders into a Shader Program, saving the program id*/
    ShaderProgram(std::string f_path, std::string v_path, std::string g_path = "")
    {
        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        std::string vertexShaderString = readFile(v_path.c_str());
        const char* vertexShaderSource = vertexShaderString.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::string fragmentShaderString = readFile(f_path.c_str());
        const char* fragmentShaderSource = fragmentShaderString.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // geometry shader
        int geometryShader;
        std::string geometryShaderString;
        //if (!g_path.empty()) {
        if (g_path != "") {
            geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
            geometryShaderString = readFile(g_path.c_str());
            const char* geometryShaderSource = geometryShaderString.c_str();
            glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
            glCompileShader(geometryShader);
            
            // check for shader compile errors
            glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }
        
        // link shaders
        int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        if (!g_path.empty()) {
            glAttachShader(shaderProgram, geometryShader);
        }
        glLinkProgram(shaderProgram);
        id = shaderProgram;
        
        // check for linking errors
        glGetProgramiv(vertexShader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
        }
        glGetProgramiv(fragmentShader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
        }
        if (!g_path.empty()) {
            glGetProgramiv(geometryShader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::GEOMETRY::PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
            }
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (!g_path.empty()) {
            glDeleteShader(geometryShader);
        }

        
    }
};
#endif /* _SHADER_PROGRAM_H */