#ifndef BMP_ADAPTOR_H
#define BMP_ADAPTOR_H

class BMP;
class Image;

class BmpAdaptor
{
public:
    BmpAdaptor();
    ~BmpAdaptor();

    Image getImage() const;

    bool captureWindow(int hwndId);
    void setBitCountWidthHeightBuf(unsigned int bit_count, unsigned int width, unsigned int height, unsigned char* Buf_);

private:
    BmpAdaptor(const BmpAdaptor&) = delete;
    BmpAdaptor& operator=(const BmpAdaptor&) = delete;


    const BMP& getBmp() const;
    void setBuf(unsigned char* Buf);
    bool isValid() const { return isValid_; }

    bool isValid_;
    unsigned int bit_count_;
    unsigned int width_;
    unsigned int height_;
    unsigned char* Buf_;
    const BMP* bmp;
};

#endif
