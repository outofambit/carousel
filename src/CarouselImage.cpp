//
//  CarouselImage.cpp
//  carousel
//
//  Created by Nicholas Masso on 3/12/14.
//
//

#include "CarouselImage.h"

using namespace ci;

CarouselImage::CarouselImage()
{
    setShouldDraw( true );
}

void CarouselImage::setup(const ci::fs::path p)
{
    mTexture = gl::Texture( loadImage( p ) );
    mBR = Vec2f( mTexture.getWidth(), mTexture.getHeight() );
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
    mArea.offsetCenterTo( mPos.value() );
}

void CarouselImage::draw()
{
    if (getShouldDraw() && mTexture)
        gl::draw( mTexture, mArea );
}

void CarouselImage::setShouldDraw(const bool b)
{
    shouldDraw = b;
}

bool CarouselImage::getShouldDraw()
{
    return shouldDraw;
}