#include "color.h"

#include <algorithm>

static int line_corellation(const RGBColor& a, const RGBColor& b)
{
    return -(abs(a.getRed() - b.getRed()) + abs(a.getGreen() - b.getGreen()) + abs(a.getBlue() - b.getBlue()));
}
static int quad_corellation(const RGBColor& a, const RGBColor& b)
{
    return -((a.getRed() - b.getRed())*(a.getRed() - b.getRed()) + (a.getGreen() - b.getGreen())*(a.getGreen() - b.getGreen()) + (a.getBlue() - b.getBlue())*(a.getBlue() - b.getBlue()));
}
static int mult_corellation(const RGBColor& a, const RGBColor& b)
{
    return a.getRed()*b.getRed() + a.getGreen()*b.getGreen() + a.getBlue()*b.getBlue();
}

int (*const RGBColor::color_corellation[CorFuncN])(const RGBColor&, const RGBColor&) = { line_corellation, quad_corellation };

static int line_corellation(const MonoColor a, const MonoColor b)
{
	return -abs(a.getIntensity() - b.getIntensity());
}
int quad_corellation(const MonoColor a, const MonoColor b)
{
    return -(a.getIntensity() - b.getIntensity())*(a.getIntensity() - b.getIntensity());
}

int (*const MonoColor::color_corellation[CorFuncN])(const MonoColor, const MonoColor) = { line_corellation, quad_corellation };
