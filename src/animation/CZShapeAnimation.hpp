//
//  CZShapeAnimation.hpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#ifndef CZShapeAnimation_hpp
#define CZShapeAnimation_hpp

#include "CZAnimation.hpp"
#include "../shape/CZShape.h"

class CZShapeAnimation : public CZAnimation
{
public:
    void update(long time) override;
    void start(std::string &name) override;
    
    CZShape *shape;
};

#endif /* CZShapeAnimation_hpp */
