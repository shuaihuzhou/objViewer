//
//  CZNode.h
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 Founder. All rights reserved.
//

#ifndef CZNode_h
#define CZNode_h

#include "CZMat4.h"
#include "CZShader.h"
#include "CZDefine.h"
#include <map>
#include <string>

class CZNode
{
public:
    // define type
    typedef enum _NodeType {
        kEmpty,                     ///< empty
        kObjModel,                  ///< obj mode
        kShape                      ///< shape
    } NodeType;
    
    typedef std::map<std::string,CZNode*> NodeMap;
    
    CZNode(NodeType t = kEmpty);
    virtual ~CZNode();
    
    virtual void resetMatrix();
    
    NodeType getType(){ return _type;}
    
<<<<<<< HEAD
    virtual void draw(CZShader *pShader){};
		virtual void draw1(ShZshader* pShader){};
		virtual void unfold(){};
=======
    // get transform matrix
    CZMat4 getTransformMat();
    
    // operate hierarchy
    bool addSubNode(std::string &name,CZNode *node);
    bool removeSubNode(std::string &name);
    CZNode * getNode(std::string &name);
    const NodeMap & getAllSubNodes();
    bool removeAllSubNodesOfType(NodeType type);
    
    virtual bool draw(CZShader *pShader, CZMat4 &viewProjMat);
>>>>>>> 7b0c7bd2f1c9219c601e952f5ce54f93311c95ae
    
    //// properties
    CZMat4 rotateMat, translateMat, scaleMat;
    CZNode *parentNode;

protected:
    NodeType _type;
    NodeMap _childrenNodes;
    
    GLuint m_vao;
    GLuint m_vboPos;
    GLuint m_vboNorm;
    GLuint m_vboTexCoord;
};

#endif /* CZNode_h */