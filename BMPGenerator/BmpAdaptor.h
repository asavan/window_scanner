#ifndef BMP_ADAPTOR_H
#define BMP_ADAPTOR_H
#include "../Scanner/image.h"
class BmpAdaptor
{
public:
	BmpAdaptor();
	~BmpAdaptor();
	void setBitCount(unsigned int bit_count) {bit_count_ = bit_count;}
	void setWidth(unsigned int width) { width_ = width;}
	void setHeight(unsigned int height) { height_ = height;}
	void setBuf(unsigned char* Buf);
	bool isValid() const { return isValid_;}
	BMP getBmp() const;	
private:
	BmpAdaptor(const BmpAdaptor&);
	BmpAdaptor& operator=(const BmpAdaptor&);
	
	bool isValid_;
    unsigned int bit_count_;
    unsigned int width_;
    unsigned int height_;
	unsigned char* Buf_;
};

#endif
