#ifndef BMP_ADAPTOR_H
#define BMP_ADAPTOR_H
class BMP;
class BmpAdaptor
{
public:
	BmpAdaptor();
	~BmpAdaptor();
	void setBitCountWidthHeightBuf(unsigned int bit_count, unsigned int width, unsigned int height, unsigned char* Buf_);
	bool isValid() const { return isValid_;}
	const BMP& getBmp() const;	
private:
	BmpAdaptor(const BmpAdaptor&);
	BmpAdaptor& operator=(const BmpAdaptor&);

	void setBuf(unsigned char* Buf);
	
	bool isValid_;
    unsigned int bit_count_;
    unsigned int width_;
    unsigned int height_;
	unsigned char* Buf_;
	const BMP* bmp;
};

#endif
