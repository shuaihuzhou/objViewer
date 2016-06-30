
#include "CZObjModel.h"
#include "CZDefine.h"

class Cube: public CZObjModel
{
public:
	 void draw(CZShader* pShader)
	 {
		 if(pShader == NULL)
		 {
			 LOG_ERROR("pShader is NULL!\n");
			 return;
		 }

		 GL_BIND_VERTEXARRAY(m_vao);


		 float ke[4], ka[4], kd[4], ks[4], Ns = 10.0;
		 ka[0] = 0.2;    ka[1] = 0.2;    ka[2] = 0.2;
		 kd[0] = 0.8;    kd[1] = 0.8;    kd[2] = 0.8;
		 ke[0] = 0.0;    ke[1] = 0.0;    ke[2] = 0.0;
		 ks[0] = 0.0;    ks[1] = 0.0;    ks[2] = 0.0;
		 Ns = 10.0;

		 glUniform3f(pShader->getUniformLocation("material.kd"), kd[0], kd[1], kd[2]);
		 glUniform3f(pShader->getUniformLocation("material.ka"), ka[0], ka[1], ka[2]);
		 glUniform3f(pShader->getUniformLocation("material.kd"), kd[0], kd[1], kd[2]);
		 glUniform3f(pShader->getUniformLocation("material.ke"), ke[0], ke[1], ke[2]);
		 glUniform3f(pShader->getUniformLocation("material.ks"), ks[0], ks[1], ks[2]);
		 glUniform1f(pShader->getUniformLocation("material.Ns"), Ns);

		 int hasTex = 0;

		 glUniform1i(pShader->getUniformLocation("hasTex"), hasTex);
		 glUniform1i(pShader->getUniformLocation("tex"), 0);



		 glDrawArrays(GL_TRIANGLES, (GLint)0, (GLsizei)pGeometry->vertNum);
		 GL_BIND_VERTEXARRAY(0);
	 }
};