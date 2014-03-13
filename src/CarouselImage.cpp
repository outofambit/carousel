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
}

void CarouselImage::setup()
{
    mTexture = gl::Texture( loadImage( app::loadAsset( "msi.jpg" ) ) );
    mBR = Vec2f( mTexture.getWidth(), mTexture.getHeight() );
}

void CarouselImage::setPos(Vec2f new_pos)
{
    app::timeline().apply( &mPos, new_pos, 0.35f, EaseOutQuint() );
}

void CarouselImage::update()
{
    mArea.set(0, 0, mBR.value().x, mBR.value().y);
    mArea.offsetCenterTo(mPos.value());
}

void CarouselImage::draw()
{
    if (mTexture)
    gl::draw( mTexture, mArea );
}