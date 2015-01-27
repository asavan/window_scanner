#include "image.h"

static const unsigned int MaxPos = 0xffffffff;

BMP::BMP(const unsigned char*const bmp_file)
{
    // BITMAPFILEHEADER
    // {
    //     WORD  bfType;
    //if (bmp_file[0] != 'B' or bmp_file[1] != 'M')
    //    throw bad_execution("unknown file format");
    //     DWORD bfSize;
    //int bmp_size =  bmp_file[2] + (bmp_file[3] << 8) + (bmp_file[4] << 16) + (bmp_file[5] << 24);
    //     WORD  bfReserved1;
    //     WORD  bfReserved2;
    //     DWORD bfOffBits;
    bitmap_ = bmp_file +  bmp_file[10] + (bmp_file[11] << 8) + (bmp_file[12] << 16) + (bmp_file[13] << 24);
    // };
    // BITMAPINFOHEADER
    // {
    //     DWORD biSize;
    //     LONG  biWidth;
    width_ = bmp_file[18] + (bmp_file[19] << 8) + (bmp_file[20] << 16) + (bmp_file[21] << 24);
    //     LONG  biHeight;
    height_ = bmp_file[22] + (bmp_file[23] << 8) + (bmp_file[24] << 16) + (bmp_file[25] << 24);
    //     WORD  biPlanes;
    //     WORD  biBitCount;
    bit_count_ = bmp_file[28];
    //     DWORD biCompression;
    //     DWORD biSizeImage;
    //     LONG  biXPelsPerMeter;
    //     LONG  biYPelsPerMeter;
    //     DWORD biClrUsed;
    //     DWORD biClrImportant;
    // }
//    std::cout << "bmp "<< width_ << " " << height_ << " " << bit_count_ << std::endl;
}

BMP::BMP(const unsigned char*const bmp_raw, unsigned int bit_count, unsigned int width, unsigned int height):
bit_count_(bit_count), width_(width), height_(height), bitmap_(bmp_raw)
{
}

RGBColor BMP::operator()(const unsigned y, const unsigned x) const
{
    const unsigned int pixel_adress = (((bit_count_/8)*width_ + 3)&0xfffffffc)*(height_ - y) + (bit_count_/8)*x;
    return RGBColor(bitmap_[pixel_adress + 2], bitmap_[pixel_adress + 1], bitmap_[pixel_adress]);
}

Image::Image(const BMP& bmp, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height):
    bmp_(bmp), height_(height), width_(width), x_(x), y_(y)
{
}

RGBColor Image::operator()(const unsigned y, const unsigned x) const 
{ 
	return bmp_(y_ + y, x_ + x);
	// return pixels_[width_*y + x]; 
}

Image::Image(const Image& im, const unsigned x, const unsigned y, const unsigned width, const unsigned height) : bmp_(im.bmp_), width_(width), height_(height), x_(im.x_ + x), y_(im.y_ + y) 
{

}

Image Image::clip(const unsigned x, const unsigned y, unsigned width, unsigned height) const
{
    if (width == MaxPos) width = width_ - x;
    if (height == MaxPos) height = height_ - y;
//    std::cout << "clip " << width_ << " " << height_ << " " << x << " " << y << " " << width << " " << height << std::endl;
    Image image(*this, x, y, height, width);
    return image;
}

std::ostream& operator<<(std::ostream& stream, const Image& i)
{
        for (unsigned int h = 0; h < i.getHeigth(); ++h)
        {
            for (unsigned int w = 0; w < i.getWidth(); ++w)
                stream << i(h, w) << " ";
            stream << std::endl;
        }

    return stream;
}
