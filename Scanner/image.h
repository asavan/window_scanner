#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"

class BMP
{
public:
    BMP(const unsigned char*const bmp_file);

    RGBColor operator()(const unsigned y, const unsigned x) const
    {
        const unsigned int pixel_adress = (((bit_count_/8)*width_ + 3)&0xfffffffc)*(height_ - y) + (bit_count_/8)*x;
        return RGBColor(bitmap_[pixel_adress + 2], bitmap_[pixel_adress + 1], bitmap_[pixel_adress]);
    }
private:
	BMP(const BMP&);
	BMP& operator=(const BMP&);
    const unsigned char*bitmap_;
    unsigned int bit_count_;
    unsigned int width_;
    unsigned int height_;
};

class Image;

std::ostream& operator<<(std::ostream& stream, const Image& i);

class Image
{
public:    
    Image(const BMP& bmp, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height);
	Image (const Image& im, const unsigned x, const unsigned y, const unsigned width, const unsigned height);
    unsigned int getHeigth() const { return height_; }
    unsigned int getWidth() const { return width_; }
    RGBColor operator()(const unsigned y, const unsigned x) const;
    Image clip(const unsigned x, const unsigned y, const unsigned width, const unsigned height) const;
private:	
	const BMP& bmp_;
    const unsigned int height_;
    const unsigned int width_;
	const unsigned int x_; 
	const unsigned int y_;
};

#endif
