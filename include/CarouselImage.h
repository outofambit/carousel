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
    
    ci::gl::Texture             mTexture, mTitleTex, mNamesTex, mZoomTex, mGradeTex;
    ci::fs::path                mBasePath;
    ci::Anim<ci::Vec2f>         mPos, mBR;
    ci::Anim<ci::ColorA>        mTitleColor, mNamesColor, mPhotoColor;
    ci::Vec2f                   mOrigSize;
    ci::Rectf                   mPhotoRect, mNamesRect, mTitleRect;
    ci::Area                    mNamesSrcArea;
    int                         mYear;
    float                       mOriginalWidth;
    bool                        mShouldDraw, mShouldDrawText, mReappearing, mResizing;
    int                         mResizeRange, mPrevResizeRange;
    
public:
    CarouselImage( const ci::fs::path p );
    void setup();
    void update();
	void draw();
    void prepToReappear();
    void setPos( const ci::Vec2f new_pos );
    void incPosNow( const ci::Vec2f amt );
    ci::Anim<ci::ColorA> * setShouldDrawText ( const bool b, ci::Anim<ci::ColorA> * triggerPtr = NULL );
    // size getters and setters
    void setWidth( const float width );
    void setWidthNow( const float new_width );
    float getWidth() const;
    void setHeight( const float height );
    float getHeight() const;
    void setWidthOrHeight ( const ci::Vec2f w_by_h );
    ci::Anim<ci::ColorA> * getPhotoAnimColor();
    void setShouldDraw( const bool b );
    bool getShouldDraw() const;
    int getYear() const;
    // hitchecks
    bool hitCheck( const ci::Vec2f pt ) const;
    bool namesHitCheck( const ci::Vec2f pt ) const;
    // for pinch to zoom
    void resizePhoto( const float inflate_amt );
    void resetPhotoSize();
    bool getResizing() const;
    void updateResizeRange();
    // for scrolling interaction of names area
    void offsetNamesArea( ci::Vec2f amt);
    // convenience methods for loading/getting textures
    bool getPhotoTexture(ci::gl::Texture *into);
    bool getTitleTexture(ci::gl::Texture *into);
    bool getNamesTexture(ci::gl::Texture *into);
};

#endif /* defined(__carousel__CarouselImage__) */
