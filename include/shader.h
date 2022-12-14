#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	unsigned int ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	void SetFloat(const char* name, float value);
	void SetBool(const char* name, bool value);
	void SetMatrix(const char* name, glm::mat4 value);
};
#endif