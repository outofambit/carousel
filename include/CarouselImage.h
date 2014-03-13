//
//  CarouselImage.h
//  carousel
//
//  Created by Nicholas Masso on 3/12/14.
//
//

#ifndef __carousel__CarouselImage__
#define __carousel__CarouselImage__

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/Timeline.h"
#include <iostream>

class CarouselImage {
    
    ci::gl::Texture             mTexture;
    ci::Anim<ci::Vec2f>         mPos, mBR;
    ci::Vec2f                   mOrigSize;
    ci::Rectf                   mArea;
    int                         mYear;
    
public:
    CarouselImage();
    void setup();
    void update();
	void draw();
    void setPos (ci::Vec2f new_pos);
    void setWidth (float width);
};

#endif /* defined(__carousel__CarouselImage__) */
