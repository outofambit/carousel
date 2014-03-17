//
//  CarouselImage.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/12/14.
//
//

#include "CarouselImage.h"

using namespace ci;

CarouselImage::CarouselImage(const fs::path p)
{
    mBasePath = p;
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

void CarouselImage::setPos(const Vec2f new_pos)
{
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

void CarouselImage::update()
{
    mArea.set(0, 0, mBR.value().x, mBR.value().y);
    mArea.offsetCenterTo( mPos );
    
}

void CarouselImage::draw()
{
    if (getShouldDraw())
    {
        gl::color(1.0, 1.0, 1.0, 0.5);
        if ( mTexture )
            gl::draw( mTexture, mArea );
        
        gl::color(mTitleColor);
        if ( mTitleTex )
            gl::draw(mTitleTex, mPos.value() + Vec2f(0, -200));
        
        gl::color(mNamesColor);
        if ( mNamesTex )
            gl::draw(mNamesTex, mPos.value() + Vec2f(0, 200));
        
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

void CarouselImage::setShouldDrawText( const bool b )
{
    if ( b != mShouldDrawText )
    {
        if (b)
        {
            app::timeline().apply( &mTitleColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint());
            app::timeline().apply( &mNamesColor, ColorA(1,1,1,1), 0.5f, EaseInOutQuint()).appendTo( &mTitleColor );
        }
        else
        {
            app::timeline().apply( &mTitleColor, ColorA(1,1,1,0), 0.5f, EaseInOutQuint());
            app::timeline().apply( &mNamesColor, ColorA(1,1,1,0), 0.5f, EaseInOutQuint());
        }
        mShouldDrawText = b;
    }
}