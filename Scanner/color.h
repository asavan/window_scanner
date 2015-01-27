#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#ifndef _MSC_VER
#include <stdint.h>
#else
typedef __int64 int64_t;
typedef __int64 uint64_t;
#endif

static const int MaxColor = 256;
static const int CorFuncN = 3;

enum CorellationType { LinearCorellation, QuadraticCorellation, MultiplyingCorellation };
inline enum CorellationType operator++(enum CorellationType& type) { return type = static_cast<CorellationType>(static_cast<int>(type) + 1); }

class RGBColor
{
public:
    RGBColor(const int64_t red = 0, const int64_t green = 0, const int64_t blue = 0): red_(red), green_(green), blue_(blue) { }

    RGBColor& operator+=(const RGBColor& summand) { red_ += summand.red_; green_ += summand.green_; blue_ += summand.blue_; return *this; }
    RGBColor operator+(const RGBColor& summand) const { RGBColor result = *this; return result += summand; }

    RGBColor& operator-=(const RGBColor& substrahend) { red_ -= substrahend.red_; green_ -= substrahend.green_; blue_ -= substrahend.blue_; return *this; }
    RGBColor operator-(const RGBColor& substrahend) const { RGBColor result = *this; return result -= substrahend; }

    RGBColor& operator*=(const int factor) { red_ *= factor; green_ *= factor; blue_ *= factor; return *this; }
    RGBColor operator*(const int factor) const { RGBColor result = *this; return result *= factor; }

    RGBColor& operator/=(const int divisor) { red_ /= divisor; green_ /= divisor; blue_ /= divisor; return *this; }
    RGBColor operator/(const int divisor) const { RGBColor result = *this; return result /= divisor; }

    int64_t getIntensity(void) const { return (red_ + green_ + blue_)/3; }
    int64_t getRed(void) const { return red_;}
    int64_t getGreen(void) const { return green_;}
    int64_t getBlue(void) const { return blue_;}

    RGBColor getInverted(void) const { return RGBColor(255 - red_, 255 - green_, 255 - blue_); }

    static int64_t (*const color_corellation[CorFuncN])(const RGBColor&, const RGBColor&);

    friend std::ostream& operator<<(std::ostream&, const RGBColor&);
private:
    int64_t red_;
    int64_t green_;
    int64_t blue_;
};

inline std::ostream& operator<<(std::ostream& stream, const RGBColor& c) { return stream << "(" << c.red_ << "," << c.green_ << "," << c.blue_ << ")"; }

class MonoColor
{
public:
    MonoColor(const int intensity = 0): intensity_(intensity) { }
    MonoColor(const RGBColor rgb_color): intensity_(rgb_color.getIntensity()) { }

    MonoColor& operator+=(const MonoColor& summand) { intensity_ += summand.intensity_; return *this; }
    MonoColor operator+(const MonoColor& summand) const { MonoColor result = *this; return result += summand; }

    MonoColor& operator-=(const MonoColor& substrahend) { intensity_ -= substrahend.intensity_; return *this; }
    MonoColor operator-(const MonoColor& substrahend) const { MonoColor result = *this; return result -= substrahend; }

    MonoColor& operator*=(const int factor) { intensity_ *= factor; return *this; }
    MonoColor operator*(const int factor) const { MonoColor result = *this; return result *= factor; }

    MonoColor& operator/=(const int divisor) { intensity_ /= divisor; return *this; }
    MonoColor operator/(const int divisor) const { MonoColor result = *this; return result /= divisor; }

    int64_t getIntensity(void) { return intensity_; }
    int64_t getRed(void) { return intensity_;}
    int64_t getGreen(void) { return intensity_;}
    int64_t getBlue(void) { return intensity_;}

    MonoColor getInverted(void) const { return MonoColor(255 - intensity_); }

    static int64_t line_corellation(const MonoColor a, const MonoColor b);
    static int64_t quad_corellation(const MonoColor a, const MonoColor b);
    static int64_t mult_corellation(const MonoColor a, const MonoColor b);

    static int64_t (*const color_corellation[CorFuncN])(const MonoColor, const MonoColor);

    friend std::ostream& operator<<(std::ostream&, const MonoColor&);

private:
    int64_t intensity_;
};

inline std::ostream& operator<<(std::ostream& stream, const MonoColor& c) { return stream << "(" << c.intensity_ << ")"; }

#endif
