/************************************************************************/
/* shader.h
/* Target: initialize VBO and compile the shader
/* version: 1.0.0
/* Author: Shuaihu Zhou
/* Date  : 2016/7/5                                                     
/************************************************************************/
#ifndef _SHZSHADER_H
#define _SHZSHADER_H

#include <iostream>
#include <fstream>
#include "glew.h"
#include <sstream>
#include <string>
#include "CZShader.h"
using namespace std;

class ShZshader
{
public:
	GLuint programShader;
	ShZshader(const GLchar* vertexPath, const GLchar* fragmentPath );
	~ShZshader();
	void begin();
	void end();
private:
};



#endif // !_SHZSHADER_H
