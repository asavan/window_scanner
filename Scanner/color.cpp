#include "color.h"

#include <algorithm>

static int64_t line_corellation(const RGBColor& a, const RGBColor& b)
{
    return -(abs(a.getRed() - b.getRed()) + abs(a.getGreen() - b.getGreen()) + abs(a.getBlue() - b.getBlue()));
}
static int64_t quad_corellation(const RGBColor& a, const RGBColor& b)
{
    return -((a.getRed() - b.getRed())*(a.getRed() - b.getRed()) + (a.getGreen() - b.getGreen())*(a.getGreen() - b.getGreen()) + (a.getBlue() - b.getBlue())*(a.getBlue() - b.getBlue()));
}
static int64_t mult_corellation(const RGBColor& a, const RGBColor& b)
{
    return a.getRed()*b.getRed() + a.getGreen()*b.getGreen() + a.getBlue()*b.getBlue();
}

int64_t (*const RGBColor::color_corellation[CorFuncN])(const RGBColor&, const RGBColor&) = { line_corellation, quad_corellation, mult_corellation };

int64_t MonoColor::line_corellation(const MonoColor a, const MonoColor b)
{
    return -abs(a.intensity_ - b.intensity_);
}
int64_t MonoColor::quad_corellation(const MonoColor a, const MonoColor b)
{
    return -(a.intensity_ - b.intensity_)*(a.intensity_ - b.intensity_);
}
int64_t MonoColor::mult_corellation(const MonoColor a, const MonoColor b)
{
    return a.intensity_*b.intensity_;
}
int64_t (*const MonoColor::color_corellation[CorFuncN])(const MonoColor, const MonoColor) = { line_corellation, quad_corellation, mult_corellation };
