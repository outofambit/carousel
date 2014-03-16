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
    bool                        shouldDraw;
    
public:
    CarouselImage();
    void setup(const ci::fs::path p);
    void update();
	void draw();
    void setPos (const ci::Vec2f new_pos);
    void setWidth (const float width);
    float getWidth();
    void setShouldDraw(const bool b);
    bool getShouldDraw();
};

#endif /* defined(__carousel__CarouselImage__) */
