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