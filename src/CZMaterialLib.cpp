#include "CZMaterialLib.h"
#include "CZLog.h"

#include <string>
using namespace std;

void CZMaterialLib::parseLine(ifstream& ifs, const string& ele_id)
{
	if ("newmtl" == ele_id) {
		string mtlName;
		ifs >> mtlName;
		LOG_INFO("newmtl %s\n",mtlName.c_str());

		CZMaterial *pNewMtl = new CZMaterial();
		m_materials.insert(pair<string, CZMaterial*>(mtlName, pNewMtl));
		m_pCur = pNewMtl;
	}
	else if ("Ns" == ele_id) {	//shininess
		ifs >> m_pCur->Ns;
	}
	else if ("Ka" == ele_id) {	//ambient color
		ifs >> m_pCur->Ka[0] >> m_pCur->Ka[1] >> m_pCur->Ka[2];
		ifs.clear();
	}
	else if ("Kd" == ele_id) {	//diffuse color
		ifs >> m_pCur->Kd[0] >> m_pCur->Kd[1] >> m_pCur->Kd[2];
		ifs.clear();
	}
	else if ("Ks" == ele_id) {	//specular color
		ifs >> m_pCur->Ks[0] >> m_pCur->Ks[1] >> m_pCur->Ks[2];
		ifs.clear();
	}
	else if ("map_Kd" == ele_id) {
		string texImgName,texImgPath;
		ifs >> texImgName;//����ͼ���·�����Ը�mtl�ļ�����Ŀ¼Ϊ����

		texImgPath = curDirPath + "/" + texImgName;//ת��������ڳ����Ŀ¼�����·��
		bool success = m_pCur->loadTexture(texImgPath.c_str());
		if(success)	LOG_INFO(" texture(%s) loaded successfully\n",texImgName.c_str());
	}
	else
		skipLine(ifs);
}

CZMaterial* CZMaterialLib::get(string &name)
{
	auto iterMtl = m_materials.find(name);
	return iterMtl != m_materials.end() ? iterMtl->second : nullptr;
}

const map<std::string, CZMaterial*>& CZMaterialLib::getAll()
{
	return m_materials;
}

CZMaterialLib::~CZMaterialLib()
{
	for (auto iterMtl = m_materials.begin(); iterMtl != m_materials.end(); iterMtl++)
	{
		delete iterMtl->second;
	}
}