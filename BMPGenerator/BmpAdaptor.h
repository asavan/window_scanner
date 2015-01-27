#ifndef BMP_ADAPTOR_H
#define BMP_ADAPTOR_H
#include "../Scanner/image.h"
class BmpAdaptor
{
public:
	BmpAdaptor() : isValid_(false){};
	void setBitCount(unsigned int bit_count) {bit_count_ = bit_count;}
	void setWidth(unsigned int width) { width_ = width;}
	void setHeight(unsigned int height) { height_ = height;}
	void setBuf(unsigned char* Buf);
	bool isValid() const { return isValid_;}
	BMP getBmp() const;
	~BmpAdaptor();
private:
	unsigned char* Buf_;
	bool isValid_;
	BmpAdaptor(const BmpAdaptor&);
	BmpAdaptor& operator=(const BmpAdaptor&);
    const unsigned char*bitmap_;
    unsigned int bit_count_;
    unsigned int width_;
    unsigned int height_;
};

#endif
