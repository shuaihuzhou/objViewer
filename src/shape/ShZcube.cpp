#include "ShZcube.h"
using namespace std;

GLubyte index1[]={
	0,1,1,2,2,3,3,0 //left
};
GLubyte index2[]={
	0,3,3,7,7,4,4,0 //back
};
GLubyte index3[]={
	4,7,7,6,6,5,5,4 //right
};
GLubyte index4[]={
	1,2,2,6,6,5,5,1 //front
};
GLubyte index5[]={
	2,3,3,7,7,6,6,2 //up
};
GLubyte index6[]={
	0,1,1,5,5,4,4,0 //down
};
GLfloat vertices[24] ={
	-0.125,-0.125,-0.125, //vertex 0
	-0.125,-0.125,0.125, //vertex 1
	-0.125,0.125,0.125, //vertex 2
	-0.125,0.125,-0.125, //vertex 3
	0.125,-0.125,-0.125, //vertex 4
	0.125,-0.125,0.125, //vertex 5
	0.125,0.125,0.125, //vertex 6
	0.125,0.125,-0.125 //vertex 7
};

cube::cube()
{
	m_current = 0;
	m_start = clock();
	m_current = 0;
	m_totalTime = 3;
	m_anglel = 0;
	m_angler = 0;
	m_anglef = 0;
	m_angleb = 0;
	m_angleu = 0;
	m_angled = 0;
	L = 0.2;
	H = 0.3;
	W = 0.5;
	m_view.SetLookAt(L/2,H/2,W/2,-L/2,-H/2,-W/2,0,1,0);
}

cube::~cube()
{

}

void cube::setTotalTime(double totalTime)
{
	m_totalTime = totalTime;
}

void cube::init()
{

	//init the vertices;
	m_vertices.push_back(CZPoint3D(-L/2, -H/2, -W/2)); //vertex 0
	m_vertices.push_back(CZPoint3D(-L/2, -H/2, W/2)); //vertex 1
	m_vertices.push_back(CZPoint3D(-L/2, H/2, W/2)); //vertex 2
	m_vertices.push_back(CZPoint3D(-L/2, H/2, -W/2)); //vertex 3
	m_vertices.push_back(CZPoint3D(L/2, -H/2, -W/2)); //vertex 4
	m_vertices.push_back(CZPoint3D(L/2, -H/2, W/2)); //vertex 5
	m_vertices.push_back(CZPoint3D(L/2, H/2, W/2)); //vertex 6
	m_vertices.push_back(CZPoint3D(L/2, H/2, -W/2)); //vertex 7



	//init VAO VBO
	//glGenVertexArrays(1,&m_vao);
	glGenBuffers(1,&m_vbo);

	//glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3 * sizeof(GL_FLOAT),m_vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//glDrawArrays(GL_TRIANGLE_FAN,0,8);

	/*glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);*/

	//glBindBuffer(GL_ARRAY_BUFFER,0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	//glBindVertexArray(0);
}

void cube::draw1(ShZshader *pShader)
{
	pShader->begin();
	//glBindVertexArray(m_vao);
	drawfacel(pShader);
	drawfacer(pShader);
	drawfacef(pShader);
	drawfaceb(pShader);
	drawfaceu(pShader);
	drawfaced(pShader);
	pShader->end();
}

void cube::unfold()
{
	unfoldl();
	unfoldr();
	unfoldf();
	unfoldb();
	unfoldu();
	unfoldd();
	//glutPostRedisplay();
}

void cube::unfoldl()
{
	
	m_current = clock();
	if ((m_current - m_start) < CLOCKS_PER_SEC * m_totalTime)
	{
		CZMat4 temp1,temp2;
		m_model_L.LoadIdentity();

		m_anglel = (m_current - m_start)/CLOCKS_PER_SEC/m_totalTime * 90;
		
		m_model_L.SetTranslation(VECTOR3D(L/2,0,-W/2));
		temp1 = m_model_L;
		m_model_L.SetRotationAxis(m_anglel,VECTOR3D(0,1,0));
		temp2 = m_model_L;
		m_model_L.SetTranslation(VECTOR3D(-L/2,0,W/2));
		m_model_L = m_model_L * temp2 * temp1;
		
		glutPostRedisplay();
	}

}

void cube::unfoldr()
{
	m_current = clock();
	if ((m_current - m_start) < CLOCKS_PER_SEC * m_totalTime)
	{
		CZMat4 temp1,temp2;
		m_model_R.LoadIdentity();

		m_angler = (m_current - m_start)/CLOCKS_PER_SEC/m_totalTime * 90;

		m_model_R.SetTranslation(VECTOR3D(-L/2,0,-W/2));
		temp1 = m_model_R;
		m_model_R.SetRotationAxis(-m_anglel,VECTOR3D(0,1,0));
		temp2 = m_model_R;
		m_model_R.SetTranslation(VECTOR3D(L/2,0,W/2));
		m_model_R = m_model_R * temp2 * temp1;
		
		glutPostRedisplay();
	}

}

void cube::unfoldf()
{

}

void cube::unfoldb()
{
	m_current = clock();
	if ((m_current - m_start) < CLOCKS_PER_SEC * m_totalTime * 2 && (m_current - m_start) > CLOCKS_PER_SEC * m_totalTime)
	{
		CZMat4 temp1,temp2;
		m_model_B.LoadIdentity();
		
		m_angleb = (m_current - (m_start + CLOCKS_PER_SEC * m_totalTime) )/CLOCKS_PER_SEC/m_totalTime * 90;

		m_model_B.SetTranslation(VECTOR3D(0,-H/2,W/2));
		temp1 = m_model_B;
		m_model_B.SetRotationAxis(m_angleb,VECTOR3D(1,0,0));
		temp2 = m_model_B;
		m_model_B.SetTranslation(VECTOR3D(0,H/2,-W/2));
		m_model_B = m_model_B * temp2 * temp1;

		glutPostRedisplay();
	}

	if ((m_current - m_start) < CLOCKS_PER_SEC * m_totalTime * 3 && (m_current - m_start) > CLOCKS_PER_SEC * m_totalTime * 2)
	{
		CZMat4 temp1,temp2,temp3,temp4,temp5;
		m_model_B.LoadIdentity();
		
		m_angleb = (m_current - (m_start + CLOCKS_PER_SEC * m_totalTime * 2) )/CLOCKS_PER_SEC/m_totalTime * 90;

		m_model_B.SetTranslation(VECTOR3D(0,-H/2,W/2));
		temp1 = m_model_B;
		m_model_B.SetRotationAxis(90,VECTOR3D(1,0,0));
		temp2 = m_model_B;

		m_model_B.SetTranslation(VECTOR3D(0,0,-W));
		temp3 = m_model_B;
		m_model_B.SetRotationAxis(m_angleb,VECTOR3D(1,0,0));
		temp4 = m_model_B;

		m_model_B.SetTranslation(VECTOR3D(0,H/2,-W/2));
		temp5 = m_model_B;
		m_model_B.SetTranslation(VECTOR3D(0,0,W));

		m_model_B = m_model_B * temp5 * temp4 * temp3 * temp2 * temp1;


		glutPostRedisplay();
	}

}

void cube::unfoldu()
{
	m_current = clock();
	if ((m_current - m_start) < CLOCKS_PER_SEC * m_totalTime * 3 && (m_current - m_start) > CLOCKS_PER_SEC * m_totalTime * 2)
	{
		CZMat4 temp1,temp2;
		m_model_U.LoadIdentity();

		m_angleu = (m_current - (m_start + CLOCKS_PER_SEC * m_totalTime * 2))/CLOCKS_PER_SEC/m_totalTime * 90;

		m_model_U.SetTranslation(VECTOR3D(0,-H/2,-W/2));
		temp1 = m_model_U;
		m_model_U.SetRotationAxis(m_angleu,VECTOR3D(1,0,0));
		temp2 = m_model_U;
		m_model_U.SetTranslation(VECTOR3D(0,H/2,W/2));
		m_model_U = m_model_U * temp2 * temp1;

		glutPostRedisplay();
	}
}

void cube::unfoldd()
{
	m_current = clock();
	if ((m_current - m_start) < CLOCKS_PER_SEC * m_totalTime * 3 && (m_current - m_start) > CLOCKS_PER_SEC * m_totalTime * 2)
	{
		CZMat4 temp1,temp2;
		m_model_D.LoadIdentity();

		m_angled = (m_current - (m_start + CLOCKS_PER_SEC * m_totalTime * 2))/CLOCKS_PER_SEC/m_totalTime * 90;

		m_model_D.SetTranslation(VECTOR3D(0,H/2,-W/2));
		temp1 = m_model_D;
		m_model_D.SetRotationAxis(-m_angled,VECTOR3D(1,0,0));
		temp2 = m_model_D;
		m_model_D.SetTranslation(VECTOR3D(0,-H/2,W/2));
		m_model_D = m_model_D * temp2 * temp1;

		glutPostRedisplay();
	}
}

void cube::drawfacel(ShZshader *pShader)
{
	GLuint ebo1;
	glGenBuffers(1,&ebo1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index1),index1,GL_STATIC_DRAW);


	GLuint modelLocaiton, viewLocation, projectionLocation, coordLoaction;
	modelLocaiton = glGetUniformLocation(pShader->programShader,"model");
	glUniformMatrix4fv(modelLocaiton, 1, false, m_model_L);

	
	viewLocation = glGetUniformLocation(pShader->programShader,"view");
	glUniformMatrix4fv(viewLocation, 1, false, m_view);

	projectionLocation = glGetUniformLocation(pShader->programShader, "projection");
	glUniformMatrix4fv(projectionLocation, 1, false, m_projection);

	coordLoaction = glGetUniformLocation(pShader->programShader,"coordinate");
	glUniformMatrix4fv(coordLoaction, 1, false, m_coord_L);

	glDrawElements(GL_LINES,8,GL_UNSIGNED_BYTE,0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glDeleteBuffers(1,&ebo1);

}

void cube::drawfaceb(ShZshader *pShader)
{
	GLuint ebo2;
	glGenBuffers(1,&ebo2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index2),index2,GL_STATIC_DRAW);

	GLuint modelLocaiton, viewLocation, projectionLocation,coordLoaction ;
	modelLocaiton = glGetUniformLocation(pShader->programShader,"model");
	glUniformMatrix4fv(modelLocaiton, 1, false, m_model_B);

	viewLocation = glGetUniformLocation(pShader->programShader,"view");
	glUniformMatrix4fv(viewLocation, 1, false, m_view);

	projectionLocation = glGetUniformLocation(pShader->programShader, "projection");
	glUniformMatrix4fv(projectionLocation, 1, false, m_projection);

	coordLoaction = glGetUniformLocation(pShader->programShader,"coordinate");
	glUniformMatrix4fv(coordLoaction, 1, false, m_coord_B);

	glDrawElements(GL_LINES,8,GL_UNSIGNED_BYTE,0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glDeleteBuffers(1,&ebo2);

}

void cube::drawfacer(ShZshader *pShader)
{
	GLuint ebo3;
	glGenBuffers(1,&ebo3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index3),index3,GL_STATIC_DRAW);


	GLuint modelLocaiton, viewLocation, projectionLocation,coordLoaction ;
	modelLocaiton = glGetUniformLocation(pShader->programShader,"model");
	glUniformMatrix4fv(modelLocaiton, 1, false, m_model_R);

	viewLocation = glGetUniformLocation(pShader->programShader,"view");
	glUniformMatrix4fv(viewLocation, 1, false, m_view);

	projectionLocation = glGetUniformLocation(pShader->programShader, "projection");
	glUniformMatrix4fv(projectionLocation, 1, false, m_projection);

	coordLoaction = glGetUniformLocation(pShader->programShader,"coordinate");
	glUniformMatrix4fv(coordLoaction, 1, false, m_coord_B);

	glDrawElements(GL_LINES,8,GL_UNSIGNED_BYTE,0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glDeleteBuffers(1,&ebo3);

}

void cube::drawfacef(ShZshader *pShader)
{
	GLuint ebo4;
	glGenBuffers(1,&ebo4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index4),index4,GL_STATIC_DRAW);


	GLuint modelLocaiton, viewLocation, projectionLocation,coordLoaction;
	modelLocaiton = glGetUniformLocation(pShader->programShader,"model");
	glUniformMatrix4fv(modelLocaiton, 1, false, m_model_F);

	viewLocation = glGetUniformLocation(pShader->programShader,"view");
	glUniformMatrix4fv(viewLocation, 1, false, m_view);

	projectionLocation = glGetUniformLocation(pShader->programShader, "projection");
	glUniformMatrix4fv(projectionLocation, 1, false, m_projection);

	coordLoaction = glGetUniformLocation(pShader->programShader,"coordinate");
	glUniformMatrix4fv(coordLoaction, 1, false, m_coord_R);

	glDrawElements(GL_LINES,8,GL_UNSIGNED_BYTE,0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glDeleteBuffers(1,&ebo4);

}

void cube::drawfaceu(ShZshader *pShader)
{
	GLuint ebo5;
	glGenBuffers(1,&ebo5);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index5),index5,GL_STATIC_DRAW);


	GLuint modelLocaiton, viewLocation, projectionLocation, coordLoaction;
	modelLocaiton = glGetUniformLocation(pShader->programShader,"model");
	glUniformMatrix4fv(modelLocaiton, 1, false, m_model_U);

	viewLocation = glGetUniformLocation(pShader->programShader,"view");
	glUniformMatrix4fv(viewLocation, 1, false, m_view);

	projectionLocation = glGetUniformLocation(pShader->programShader, "projection");
	glUniformMatrix4fv(projectionLocation, 1, false, m_projection);

	coordLoaction = glGetUniformLocation(pShader->programShader,"coordinate");
	glUniformMatrix4fv(coordLoaction, 1, false, m_coord_U);

	glDrawElements(GL_LINES,8,GL_UNSIGNED_BYTE,0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glDeleteBuffers(1,&ebo5);

}

void cube::drawfaced(ShZshader *pShader)
{
	GLuint ebo6;
	glGenBuffers(1,&ebo6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index6),index6,GL_STATIC_DRAW);


	GLuint modelLocaiton, viewLocation, projectionLocation, coordLoaction;
	modelLocaiton = glGetUniformLocation(pShader->programShader,"model");
	glUniformMatrix4fv(modelLocaiton, 1, false, m_model_D);

	viewLocation = glGetUniformLocation(pShader->programShader,"view");
	glUniformMatrix4fv(viewLocation, 1, false, m_view);

	projectionLocation = glGetUniformLocation(pShader->programShader, "projection");
	glUniformMatrix4fv(projectionLocation, 1, false, m_projection);

	coordLoaction = glGetUniformLocation(pShader->programShader,"coordinate");
	glUniformMatrix4fv(coordLoaction, 1, false, m_coord_D);

	glDrawElements(GL_LINES,8,GL_UNSIGNED_BYTE,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glDeleteBuffers(1,&ebo6);

}