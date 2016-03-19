#include "CZObjModel.h"
#include "CZMaterial.h"
#include "CZLog.h"
#include "CZDefine.h"

using namespace std;

CZObjModel::CZObjModel()
{
	pCurGeometry = NULL;
    m_vao = -1;
    m_vboPos = m_vboNorm = m_vboNorm = -1;
}
CZObjModel::~CZObjModel()
{
	// geometry
	for (vector<CZGeometry*>::iterator itr = geometries.begin(); itr != geometries.end(); itr++)
	{
		delete *itr;
	}
	geometries.clear();

	clearRawData();
    vector<CZVector3D<float>> temp1;
    vector<CZVector2D<float>> temp2;
    positions.clear();
    positions.swap(temp1);
    normals.clear();
    normals.swap(temp1);
    texcoords.clear();
    texcoords.swap(temp2);
    
    // graphics
    if(m_vao != -1) GL_DEL_VERTEXARRAY(1, &m_vao);
    if(m_vboPos != -1) glDeleteBuffers(1, &m_vboPos);
    if(m_vboNorm != -1) glDeleteBuffers(1, &m_vboNorm);
    if(m_vboTexCoord != -1) glDeleteBuffers(1, &m_vboTexCoord);
    
}

bool CZObjModel::load(const string& path)
{
	LOG_INFO("Parsing %s ...\n", path.c_str());

	if(CZObjFileParser::load(path) == false) return false;
    
    unpackRawData();
    
	/// load material lib
	materialLib.load(curDirPath + "/" + mtlLibName);

	clearRawData();

	return true;
}

bool CZObjModel::saveAsBinary(const std::string& path)
{
    FILE *fp = fopen(path.c_str(), "wb");
    
    if (fp == NULL)
    {
        LOG_ERROR("file open failed\n");
        return false;
    }
    // material lib name
    unsigned char mtlLibNameLen = mtlLibName.size();
    fwrite((char*)&mtlLibNameLen, sizeof(unsigned char), 1, fp);
    fwrite((char*)mtlLibName.c_str(), sizeof(char), mtlLibNameLen, fp);
    
    // geometry
    unsigned short count = geometries.size();
    fwrite((char*)(&count), sizeof(count), 1, fp);
    
    for (vector<CZGeometry*>::iterator itr = geometries.begin(); itr != geometries.end(); itr++)
    {
        CZGeometry *p = *itr;
        // hasTexcoord
        fwrite(&(p->hasTexCoord), sizeof(bool), 1, fp);
        // material name
        unsigned char mtlNameLen = p->materialName.size();
        fwrite(&mtlNameLen, sizeof(unsigned char), 1, fp);
        fwrite(p->materialName.c_str(), sizeof(char), mtlNameLen, fp);
        
        // data
        fwrite(&(p->vertNum), sizeof(p->vertNum), 1, fp);
    }
    
    long totalVertNum = positions.size();
    fwrite(&(totalVertNum), sizeof(totalVertNum), 1, fp);
    fwrite(positions.data(), sizeof(CZVector3D<float>), totalVertNum, fp);
    fwrite(normals.data(), sizeof(CZVector3D<float>), totalVertNum, fp);
    fwrite(texcoords.data(), sizeof(CZVector2D<float>), totalVertNum, fp);
    
    fclose(fp);
    
    return true;
}

bool CZObjModel::loadBinary(const std::string& path,const char *originalPath/*  = NULL */)
{
    string strOriginalPath;
    if(originalPath) strOriginalPath = string(originalPath);
    else             strOriginalPath = path;
    
    curDirPath = strOriginalPath.substr(0, strOriginalPath.find_last_of('/'));
    
    FILE *fp = fopen(path.c_str(), "rb");
    
    if (fp == NULL)
    {
        LOG_DEBUG("there's no binary data for this model\n");
        return false;
    }
    
    unsigned char mtlLibNameLen;
    fread((char*)&mtlLibNameLen, sizeof(unsigned char), 1, fp);
    mtlLibName.resize(mtlLibNameLen);
    fread((char*)mtlLibName.c_str(), sizeof(char), mtlLibNameLen, fp);
    
    
    // geometry
    unsigned short count;
    fread((char*)(&count), sizeof(count), 1, fp);
    
    long totalVertNum = 0;
    for (int iGeo = 0; iGeo < count; iGeo++)
    {
        CZGeometry *pNewGeometry = new CZGeometry();
        
        // hasTexcoord
        fread(&(pNewGeometry->hasTexCoord), sizeof(bool), 1, fp);

        // material name
        unsigned char mtlLibNameLen;
        
        fread(&mtlLibNameLen, sizeof(unsigned char), 1, fp);
        pNewGeometry->materialName.resize(mtlLibNameLen);
        fread((char*)pNewGeometry->materialName.c_str(), sizeof(char), mtlLibNameLen, fp);
        
        // data
        long numVert;
        fread(&numVert, sizeof(numVert), 1, fp);
        
        pNewGeometry->firstIdx = totalVertNum;
        pNewGeometry->vertNum = numVert;
        totalVertNum +=  numVert;
        
        geometries.push_back(pNewGeometry);
    }
    
    
    fread(&(totalVertNum), sizeof(totalVertNum), 1, fp);
    
    positions.resize(totalVertNum);
    normals.resize(totalVertNum);
    texcoords.resize(totalVertNum);
    fread(positions.data(), sizeof(CZVector3D<float>), totalVertNum, fp);
    fread(normals.data(), sizeof(CZVector3D<float>), totalVertNum, fp);
    fread(texcoords.data(), sizeof(CZVector2D<float>), totalVertNum, fp);
    
    fclose(fp);
    
    // transform to graphic card
    
    // vao
    GL_GEN_VERTEXARRAY(1, &m_vao);
    GL_BIND_VERTEXARRAY(m_vao);
    
    // vertex
    glGenBuffers(1, &m_vboPos);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
    glBufferData(GL_ARRAY_BUFFER,positions.size() * 3 * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // normal
    glGenBuffers(1, &m_vboNorm);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // texcoord
    glGenBuffers(1, &m_vboTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * 2 * sizeof(GLfloat), texcoords.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    GL_BIND_VERTEXARRAY(0);
    CZCheckGLError();
    
    materialLib.load(curDirPath + "/" + mtlLibName);
    
    CZCheckGLError();
    
    return true;
}

void CZObjModel::draw(CZShader* pShader)
{
    if(pShader == NULL)
    {
        LOG_ERROR("pShader is NULL!\n");
        return;
    }
    
    GL_BIND_VERTEXARRAY(m_vao);

    for (vector<CZGeometry*>::iterator itr = geometries.begin(); itr != geometries.end(); itr++)
    {
        CZGeometry *pGeometry = *itr;
        CZMaterial *pMaterial = materialLib.get(pGeometry->materialName);
        
        float ka[4], kd[4];
        if (pMaterial == NULL)
        {
            ka[0] = 0.2; ka[1] = 0.2; ka[2] = 0.2;
            kd[0] = 1;   kd[1] = 1.0; kd[2] = 1.0;
            LOG_ERROR("pMaterial is NULL\n");
        }
        else
        {
            for (int i=0; i<3; i++) {
                ka[i] = pMaterial->Ka[i];
                kd[i] = pMaterial->Kd[i];
            }
        }
		glUniform3f(pShader->getUniformLocation("material.kd"), kd[0], kd[1], kd[2]);
        glUniform3f(pShader->getUniformLocation("material.ka"), ka[0], ka[1], ka[2]);
        
        int hasTex;
        if (pMaterial && pMaterial->use() && pGeometry->hasTexCoord)
            hasTex = 1;
        else	hasTex = 0;
        
        glUniform1i(pShader->getUniformLocation("hasTex"), hasTex);
        glUniform1i(pShader->getUniformLocation("tex"), 0);
        
		glDrawArrays(GL_TRIANGLES, (GLint)pGeometry->firstIdx, (GLsizei)pGeometry->vertNum);
     
    }
    
    GL_BIND_VERTEXARRAY(0);
}


void CZObjModel::clearRawData()
{
	/*free memory��
	 *��link��http://www.cppblog.com/lanshengsheng/archive/2013/03/04/198198.html��
	 */

	m_vertRawVector.clear();
    vector<CZVector3D<float>> temp;
	m_vertRawVector.swap(temp);

	m_texRawVector.clear();
	m_texRawVector.swap(temp);

	m_normRawVector.clear();
	m_texRawVector.swap(temp);
}

void CZObjModel::unpackRawData()
{
    positions.clear();
    normals.clear();
    texcoords.clear();
    
    long totalVertNum = 0;
    for (vector<CZGeometry*>::iterator itr = geometries.begin(); itr != geometries.end(); itr++)
    {
        CZGeometry *pGeometry = (*itr);
        long vertNum = pGeometry->unpackRawData(m_vertRawVector, m_normRawVector, m_texRawVector, \
                          positions,normals,texcoords);
        pGeometry->firstIdx = totalVertNum;
        totalVertNum += vertNum;
    }
    
    // transform to graphic card
    
    // vao
    GL_GEN_VERTEXARRAY(1, &m_vao);
    GL_BIND_VERTEXARRAY(m_vao);
    
    // vertex
    glGenBuffers(1, &m_vboPos);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
    glBufferData(GL_ARRAY_BUFFER,positions.size() * 3 * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // normal
    glGenBuffers(1, &m_vboNorm);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // texcoord
    glGenBuffers(1, &m_vboTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * 2 * sizeof(GLfloat), texcoords.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    GL_BIND_VERTEXARRAY(0);
}

//////////////////////////////////////////////////////////////////////////

void CZObjModel::parseLine(ifstream& ifs, const string& ele_id)
{
	if ("mtllib" == ele_id)
		parseMaterialLib(ifs);
	else if ("usemtl" == ele_id)
		parseUseMaterial(ifs);
	else if ("v" == ele_id)
		parseVertex(ifs);
	else if ("vt" == ele_id)
		parseVertexTexCoord(ifs);
	else if ("vn" == ele_id)
		parseVertexNormal(ifs);
	else if ("f" == ele_id)
		parseFace(ifs);
	else
		skipLine(ifs);
}

void CZObjModel::parseMaterialLib(std::ifstream &ifs)
{
	ifs >> mtlLibName;
	LOG_INFO("	mtllib %s \n", mtlLibName.c_str());
}

void CZObjModel::parseUseMaterial(std::ifstream &ifs)
{
	CZGeometry *pNewGeometry = new CZGeometry();
	pCurGeometry = pNewGeometry;
	ifs >> pNewGeometry->materialName;
	geometries.push_back(pNewGeometry);
	LOG_INFO("	usemtl %s\n",pNewGeometry->materialName.c_str());
}

void CZObjModel::parseVertex(std::ifstream &ifs)
{
	float x, y, z;
	ifs >> x >> y >> z;

	m_vertRawVector.push_back(CZVector3D<float>(x, y, z));
}

void CZObjModel::parseVertexNormal(std::ifstream &ifs)
{
	float x, y, z;
	ifs >> x >> y >> z;

	if (!ifs.good()) {                     // in case it is -1#IND00
		x = y = z = 0.0;
		ifs.clear();
		skipLine(ifs);
	}
	m_normRawVector.push_back(CZVector3D<float>(x, y, z));
}

void CZObjModel::parseVertexTexCoord(std::ifstream &ifs)
{
	float x, y, z;
	ifs >> x >> y >> z;
	ifs.clear();                           // is z (i.e. w) is not available, have to clear error flag.

	m_texRawVector.push_back(CZVector3D<float>(x, y, z));
}

void CZObjModel::parseFace(std::ifstream &ifs)
{
	CZFace face;
	int data[3] = { -1, -1, -1 };
	int count;

	for (int i = 0; i < 3; i++){
		count = parseNumberElement(ifs, data);
		face.addVertTexNorm(data[0], data[1], data[2]);
	}
	skipLine(ifs);

	pCurGeometry->addFace(face);

	ifs.clear();
}
