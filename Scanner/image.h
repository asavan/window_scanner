#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"

class BMP
{
public:
    explicit BMP(const unsigned char*const bmp_file);
	BMP(const unsigned char*const bmp_raw, unsigned int bit_count, unsigned int width, unsigned int height);
	RGBColor operator()(const unsigned y, const unsigned x) const;
	unsigned int getWidth() const {return width_;}
	unsigned int getHeight() const {return height_;}
private:
	BMP(const BMP&);
	BMP& operator=(const BMP&);
    const unsigned char*bitmap_;
    unsigned int bit_count_;
    unsigned int width_;
    unsigned int height_;
};


class Image
{
public:    
    Image(const BMP& bmp, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height);
	Image (const Image& im, const unsigned x, const unsigned y, const unsigned width, const unsigned height);
	static Image getImageFromBmp(const BMP& bmp);
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

std::ostream& operator<<(std::ostream& stream, const Image& i);

#endif
