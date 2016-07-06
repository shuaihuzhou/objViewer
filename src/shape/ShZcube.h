#ifndef _SHZCUBE_H
#define _SHZCUBE_H

#include "glew.h"
#include <stdlib.h>
#include "glut.h"
#include <iostream>
#include <time.h>
#include "CZNode.h"
#include "CZMat4.h"
#include <vector>
#include "ShZshader.h"
using namespace std;
class cube :public CZNode
{
public:
	cube();
	~cube();
	void init();
	void setLWH(GLuint L, GLuint H, GLuint W);
	void setTotalTime(double totalTime);
	virtual void draw1(ShZshader *pShader);
	virtual void unfold();
	void fold();
	
	clock_t m_start,m_current;
	double m_totalTime;
	double m_anglel,m_angler,m_angleb,m_angleb2,m_anglef,m_angleu,m_angled;
	double f_x,f_y,f_z,f_x2,f_y2,f_z2;
	GLuint m_vao,m_vbo;
	GLdouble L, H, W;
	vector<CZPoint3D> m_vertices;
	//GLfloat m_vertices[24];
	CZMat4 m_projection,m_view;
	CZMat4 m_model_L,m_coord_L;
	CZMat4 m_model_R,m_coord_R;
	CZMat4 m_model_F,m_coord_F;
	CZMat4 m_model_B,m_coord_B;
	CZMat4 m_model_U,m_coord_U;
	CZMat4 m_model_D,m_coord_D;
protected:
private:
	void drawfacel(ShZshader *pShader);
	void drawfacer(ShZshader *pShader);
	void drawfacef(ShZshader *pShader);
	void drawfaceb(ShZshader *pShader);
	void drawfaceu(ShZshader *pShader);
	void drawfaced(ShZshader *pShader);
	void unfoldl();
	void unfoldr();
	void unfoldb();
	void unfoldf();
	void unfoldu();
	void unfoldd();
	void foldl();
	void foldr();
	void foldb();
	void foldf();
	void foldu();
	void foldd();
};

#endif // !_SHZCUBE_H
