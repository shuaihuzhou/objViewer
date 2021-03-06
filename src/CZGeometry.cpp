#include "CZGeometry.h"
#include <cfloat>

using namespace std;

CZGeometry::CZGeometry(): aabbMin(CZVector3D<float>(FLT_MAX, FLT_MAX, FLT_MAX)), 
						  aabbMax(CZVector3D<float>(-FLT_MAX, -FLT_MAX, -FLT_MAX))
{
	hasNormal = false;
	hasTexCoord = false;
}

CZGeometry::~CZGeometry()
{
	faces.clear();
    vector<CZFace> temp;
    faces.swap(temp);
}

long CZGeometry::unpackRawData(const std::vector<CZVector3D<float> > &posRawVector,	\
            const std::vector<CZVector3D<float> > &normRawVector,	\
            const std::vector<CZVector2D<float> > &texCoordRawVector, \
            std::vector<CZVector3D<float> > &outPositions, \
            std::vector<CZVector3D<float> > &outNormals, \
            std::vector<CZVector2D<float> > &outTexcoords)
{
    vector<CZVector3D<float> > tempPositions;
    vertNum = 0;
    for (vector<CZFace>::const_iterator itr = faces.begin(); itr != faces.end(); ++itr)
    {
        for (unsigned i = 0; i < itr->v.size(); ++i)
        {
            vertNum ++ ;
            outPositions.push_back(posRawVector[itr->v[i]-1]);
            if (hasNormal)	outNormals.push_back(normRawVector[itr->vn[i]-1]);
            else            tempPositions.push_back(posRawVector[itr->v[i]-1]);
            if (hasTexCoord)	outTexcoords.push_back(texCoordRawVector[itr->vt[i]-1]);
            else				outTexcoords.push_back(CZVector2D<float>(0, 0));
        }
    }

    if (!hasNormal)
    {
        generateFaceNorm(tempPositions,outNormals);
        
        tempPositions.clear();
        vector<CZVector3D<float> > temp;
        tempPositions.swap(temp); 
    }
    
    return vertNum;
}

//////////////////////////////////////////////////////////////////////////

void CZGeometry::generateFaceNorm(vector<CZVector3D<float> > &positions,vector<CZVector3D<float> > &outNormals)
{

	long vertNum = positions.size();
	for (long iVert = 0; iVert < vertNum; iVert += 3)
	{
		const CZVector3D<float>& v1 = positions[iVert];
		const CZVector3D<float>& v2 = positions[iVert + 1];
		const CZVector3D<float>& v3 = positions[iVert + 2];

		CZVector3D<float> v(v2.x-v1.x, v2.y-v1.y, v2.z-v1.z);
		CZVector3D<float> w(v3.x-v1.x, v3.y-v1.y, v3.z-v1.z);

		CZVector3D<float> vn = v.cross(w);

		vn.normalize();

		outNormals.push_back(vn);
		outNormals.push_back(vn);
		outNormals.push_back(vn);
	}
}

/// update aabb
void CZGeometry::updateAABB(CZVector3D<float> p)
{
	if(p.x > aabbMax.x)	aabbMax.x = p.x;
	if(p.x < aabbMin.x) aabbMin.x = p.x;

	if(p.y > aabbMax.y)	aabbMax.y = p.y;
	if(p.y < aabbMin.y) aabbMin.y = p.y;

	if(p.z > aabbMax.z)	aabbMax.z = p.z;
	if(p.z < aabbMin.z) aabbMin.z = p.z;
}