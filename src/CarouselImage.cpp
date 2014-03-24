//
//  CarouselImage.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/12/14.
//
//

#include "CarouselImage.h"

using namespace ci;
using namespace std;

CarouselImage::CarouselImage(const fs::path p)  :
    mBasePath(p),
    mPhotoColor(ColorA(1,1,1,1)),
    mReappearing(false)
{
    mYear = std::atoi( p.filename().string().c_str() );
}

void CarouselImage::setup()
{
    mTexture = gl::Texture( loadImage( mBasePath / fs::path( "photo.jpg" ) ) );
    mBR = Vec2f( mTexture.getWidth(), mTexture.getHeight() );
}

gl::Texture CarouselImage::getTitleTexture()
{
    return loadImage( mBasePath / fs::path( "title.png" ) );
}

gl::Texture CarouselImage::getNamesTexture()
{
    return loadImage( mBasePath / fs::path( "names.png" ) );
}

void CarouselImage::prepToReappear()
{ mReappearing = true;  }

void CarouselImage::setPos(const Vec2f new_pos)
{
    if ( mReappearing )
    {
        app::timeline().apply(&mPhotoColor, ColorA(1,1,1,0), 0.5, EaseNone());
        app::timeline().apply(&mNamesColor, ColorA(1,1,1,0), 0.5, EaseNone());
        app::timeline().apply(&mTitleColor, ColorA(1,1,1,0), 0.5, EaseNone());
        app::timeline().apply( &mPos, new_pos, 0.35f, EaseOutQuint() ). appendTo(&mPhotoColor);
        app::timeline().appendTo(&mPhotoColor, ColorA(1,1,1,1), 0.5, EaseNone()).appendTo(&mPos);
        
        mReappearing = false;
    }
    else
        app::timeline().apply( &mPos, new_pos, 0.35f, EaseOutQuint() );
}

void CarouselImage::setWidth(const float width)
{
    const float height = width / mTexture.getAspectRatio();
    app::timeline().apply( &mBR, Vec2f(width, height), 0.25f, EaseInOutQuint());
}

float CarouselImage::getWidth()
{
    return mBR.value().x;
}

Anim<ColorA> * CarouselImage::getPhotoAnimColor()
{   return &mPhotoColor;    }

void CarouselImage::update()
{
    mArea.set(0, 0, mBR.value().x, mBR.value().y);
    mArea.offsetCenterTo( mPos );
    
}

void CarouselImage::draw()
{
    if (getShouldDraw())
    {
        gl::color(mPhotoColor);
        if ( mTexture )
            gl::draw( mTexture, mArea );
        
        gl::color(mTitleColor);
        if ( mTitleTex )
            gl::draw(mTitleTex, Vec2f(app::getWindowWidth()/2-mTitleTex.getWidth()/2, 150));
        
        gl::color(mNamesColor);
        if ( mNamesTex )
            gl::draw(mNamesTex, Vec2f(app::getWindowWidth()/2-mNamesTex.getWidth()/2, 800));
        
    }
}

void CarouselImage::setShouldDraw(const bool b)
{
    if ( b != getShouldDraw() )
    {
        if ( b )
        {
            mTitleTex = getTitleTexture();
            mNamesTex = getNamesTexture();
        }
        
        else
        {
            mTitleTex.reset();
            mNamesTex.reset();
        }
        
        mShouldDraw = b;
    }
}

bool CarouselImage::getShouldDraw()
{
    return mShouldDraw;
}

Anim<ColorA> * CarouselImage::setShouldDrawText( const bool b, Anim<ci::ColorA> * triggerPtr)
{
    if ( b != mShouldDrawText )
    {
        mShouldDrawText = b;
        if ( b )
        {
            if ( triggerPtr )
            {
                app::timeline().apply( &mTitleColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint()).appendTo( triggerPtr );
                app::timeline().apply( &mNamesColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint()).appendTo( &mTitleColor );
                return &mNamesColor;
            }
            else
            {
                app::timeline().apply( &mTitleColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint() );
                app::timeline().apply( &mNamesColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint()).appendTo( &mTitleColor );
                return &mNamesColor;
            }

        }
        else
        {
            app::timeline().apply( &mTitleColor, ColorA(1,1,1,0), 0.5f, EaseInOutQuint());
            app::timeline().apply( &mNamesColor, ColorA(1,1,1,0), 0.5f, EaseInOutQuint());
            return &mTitleColor;
        }

    }
    return NULL;
}

int CarouselImage::getYear() const
{
    return mYear;
}