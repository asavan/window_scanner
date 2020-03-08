#ifndef COLOR_H
#define COLOR_H

#include <ostream>

static const int MaxColor = 256;
static const int CorFuncN = 2;

enum CorellationType { LinearCorellation, QuadraticCorellation, MultiplyingCorellation };
inline enum CorellationType operator++(enum CorellationType& type) { return type = static_cast<CorellationType>(static_cast<int>(type) + 1); }

class RGBColor
{
public:
    RGBColor(int red = 0, int green = 0, int blue = 0): red_(red), green_(green), blue_(blue) { }

    RGBColor& operator+=(const RGBColor& summand) { red_ += summand.red_; green_ += summand.green_; blue_ += summand.blue_; return *this; }
    RGBColor operator+(const RGBColor& summand) const { RGBColor result = *this; return result += summand; }

    RGBColor& operator-=(const RGBColor& substrahend) { red_ -= substrahend.red_; green_ -= substrahend.green_; blue_ -= substrahend.blue_; return *this; }
    RGBColor operator-(const RGBColor& substrahend) const { RGBColor result = *this; return result -= substrahend; }

    RGBColor& operator*=(const int factor) { red_ *= factor; green_ *= factor; blue_ *= factor; return *this; }
    RGBColor operator*(const int factor) const { RGBColor result = *this; return result *= factor; }

    RGBColor& operator/=(const int divisor) { red_ /= divisor; green_ /= divisor; blue_ /= divisor; return *this; }
    RGBColor operator/(const int divisor) const { RGBColor result = *this; return result /= divisor; }

    int getIntensity(void) const { return (red_ + green_ + blue_)/3; }
    int getRed(void) const { return red_;}
    int getGreen(void) const { return green_;}
    int getBlue(void) const { return blue_;}

    RGBColor getInverted(void) const { return RGBColor(255 - red_, 255 - green_, 255 - blue_); }

    static int (*const color_corellation[CorFuncN])(const RGBColor&, const RGBColor&);

    friend std::ostream& operator<<(std::ostream&, const RGBColor&);
private:
    int red_;
    int green_;
    int blue_;
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

    int getIntensity(void) const { return intensity_; }
    int getRed(void) const { return intensity_;}
    int getGreen(void) const { return intensity_;}
    int getBlue(void) const { return intensity_;}

    MonoColor getInverted(void) const { return MonoColor(255 - intensity_); }

    static int (*const color_corellation[CorFuncN])(const MonoColor, const MonoColor);

    friend std::ostream& operator<<(std::ostream&, const MonoColor&);

private:
    int intensity_;
};

inline std::ostream& operator<<(std::ostream& stream, const MonoColor& c) { return stream << "(" << c.intensity_ << ")"; }

#endif
