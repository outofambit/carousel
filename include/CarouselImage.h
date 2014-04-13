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
#include "cinder/Area.h"
#include "cinder/gl/Texture.h"
#include "cinder/Timeline.h"
#include <iostream>

class CarouselImage {
    
    ci::gl::Texture             mTexture, mTitleTex, mNamesTex;
    ci::fs::path                mBasePath;
    ci::Anim<ci::Vec2f>         mPos, mBR;
    ci::Anim<ci::ColorA>        mTitleColor, mNamesColor, mPhotoColor;
    ci::Vec2f                   mOrigSize;
    ci::Rectf                   mPhotoRect, mNamesRect;
    ci::Area                    mNamesSrcArea;
    int                         mYear;
    float                       mOriginalWidth;
    bool                        mShouldDraw, mShouldDrawText, mReappearing, mResizing;
    
public:
    CarouselImage( const ci::fs::path p );
    void setup();
    void update();
	void draw();
    void prepToReappear();
    void setPos( const ci::Vec2f new_pos );
    ci::Anim<ci::ColorA> * setShouldDrawText ( const bool b, ci::Anim<ci::ColorA> * triggerPtr = NULL );
    void setWidth( const float width );
    float getWidth();
    ci::Anim<ci::ColorA> * getPhotoAnimColor();
    void setShouldDraw( const bool b );
    bool getShouldDraw();
    int getYear() const;
    // hitchecks
    bool hitCheck( const ci::Vec2f pt ) const;
    bool namesHitCheck( const ci::Vec2f pt ) const;
    // for pinch to zoom
    void resizePhoto( const float inflate_amt );
    void resetPhotoSize();
    // for scrolling interaction of names area
    void offsetNamesArea( ci::Vec2f amt);
    // convenience methods for loading/getting textures
    ci::gl::Texture getTitleTexture();
    ci::gl::Texture getNamesTexture();
};

#endif /* defined(__carousel__CarouselImage__) */
