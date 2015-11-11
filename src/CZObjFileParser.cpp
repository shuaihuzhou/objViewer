#include "CZObjFileParser.h"
#include "CZLog.h"

using namespace std;

bool CZObjFileParser::load(const string& path)
{
	ifstream ifs(path.c_str(), ios::in | ios::ate);
	if (!ifs)
	{
		LOG_WARN("%s not exist\n",path.c_str());
		return false;
	}

	curDirPath = path.substr(0, path.find_last_of('/'));

    size_t fileSize = (size_t)ifs.tellg();
	ifs.seekg(0, ios::beg);

	//	explain what's going on
	LOG_INFO("Parsing file %s  (size = %ld bytes)...\n", path.c_str(), fileSize);

	// and go.
	static size_t lastPercent = 0;
	size_t percent = 10;	//	progress indicator
	while (skipCommentLine(ifs))
	{
		// show progress for files larger than one Mo
		if ((fileSize > 1024 * 1024) && (100 * ifs.tellg() / fileSize >= percent)) {
			percent = 100 * (size_t)ifs.tellg() / fileSize;
			percent = (percent / 10) * 10;

			if (lastPercent != percent)
			{
				LOG_INFO("processing %ld%%\n", percent);
				lastPercent = percent;
			}
		}

		string ele_id;
		if (!(ifs >> ele_id))
			break;
		else
			parseLine(ifs, ele_id);
	}

	return true;
}

bool CZObjFileParser::load(const char *filename)
{
	if(filename == NULL)
	{
		LOG_WARN("filename is NULL\n");
		return false;
	}
	
	string strFilename(filename);
	return load(strFilename);
}

void CZObjFileParser::skipLine(ifstream& is)
{
	char next;
	is >> std::noskipws;
	while ((is >> next) && ('\n' != next));
}

bool CZObjFileParser::skipCommentLine(ifstream& is)
{
	char next;
	while (is >> std::skipws >> next)
	{
		is.putback(next);
		if ('#' == next)
			skipLine(is);
		else
			return true;
	}
	return false;
}

/*�÷���ifstream::operator >> (int&)
*�Ⱥ������ɿհ׷���Ȼ�������¼���������в�ͬ�����
*								|			  ��ȡ��			|		   clear()��	  
*������������ʽ��	����	|��ȡ��	good()?	eof()?	peek()	|good()?	eof()?	peek()
*<digit>+ <������>		123a	|T		T		F		a		|
*<digit>0 <������>		a		|F		F		F		EOF		|T			F		a
*<digit>+ <�ļ�β>		123β	|T		F		T		EOF		|T			F		EOF
*<digit>0 <�ļ�β>		β		|F		F		T		EOF		|T			F		EOF 
*
*ʵ��parseNumberElement()ʱ����Ҫ���������������������δ�������*/
int CZObjFileParser::parseNumberElement(ifstream &ifs, int *pData, char sep, int defaultValue, int maxCount)
{
	int count = 0;
	int data;
	char c;//���������ַ�

	/*ÿ��ѭ�������������ݣ�
	*1.count�Ƿ�+1���Ƿ�Ҫ��Ĭ��ֵ���뵱ǰ���ݴ�
	*2.�Ƿ��������һ�ַ�Ϊsep����������������
	*3.�Ƿ���Ҫifs.clear()*/
	while (true){
		ifs >> data;
		if (ifs.good()){//���������һ��<digit>+ <������>��
			pData[count++] = data;
			if (ifs.peek() != sep)
				break;
			else
				ifs.get(c);
		}
		else{
			if (!ifs.eof()){//�������������<digit>0 <������>��
				ifs.clear();
				pData[count++] = defaultValue;
				if (ifs.peek() != sep)
					break;
				else
					ifs.get(c);
			}
			else{//�����ļ�β����������ģ��������Ƿ�������ݶ����䶪��
				ifs.clear();
				break;
			}
		}
	}

	return count;
}