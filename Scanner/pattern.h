#ifndef PATTERN_H
#define PATTERN_H

// #include "image.h"
#include "color.h"
class Image;

template<typename TColor, int PatternWidth = 32, int PatternHeight = 32>
class Pattern
{
public:

    Pattern(void): grade_(0), summary_intensity_(0), average_intensity_(0) {}
    explicit Pattern(const Image& image);

    void merge(const Pattern<TColor>&);

    static int corellation(const Pattern&, const Pattern&, CorellationType);

    void dump__(const std::string& filename, const std::string& prefix) const;

private:

    static const unsigned int CorOptN = 3;
    enum CorellationOption { Centering, Normalization, Balancing };
    static const bool cor_opt_[CorFuncN][CorOptN];
    static bool is_need_opt_(CorellationType type, CorellationOption option) { return cor_opt_[static_cast<int>(type)][static_cast<int>(option)]; }

    TColor inverted_colors_matrix[PatternHeight][PatternWidth];

    static const unsigned int Max_Grade_ = 2*MaxColor;
    unsigned int grade_;

    int summary_intensity_;
    int average_intensity_;
    int intensity_center_y_;
    int intensity_center_x_;

};

template <typename TColor, int PatternWidth, int PatternHeight>
const bool Pattern<TColor, PatternWidth, PatternHeight>::cor_opt_[CorFuncN][CorOptN] =
{// Centering Normalization Balancing
     {true,       true,       false}, // LinearCorellation
     {true,       true,       false}//, // QuadraticCorellation
//     {true,       false,      true}   // MultiplyingCorellation
};

#include "pattern.tpl"

#endif













/*
class MetaPattern
{
public:
    virtual void merge(const MetaPattern&) = 0;
    virtual ~MetaPattern() { }
};

class RankPattern: public MetaPattern
{
public:
    RankPattern(const Image& image): pattern_(image.clip(0, 0, 25, 25)) { }
    virtual void merge(const MetaPattern& a) { pattern_.merge(a); }
private:
    Pattern<MonoColor> pattern_;
};

class SuitPattern: public MetaPattern
{
public:
    SuitPattern(const Image& image): pattern_(image.clip(0, 27, 25, 25)) { }
    virtual void merge(const MetaPattern& a) { pattern_.merge(a); }
private:
    Pattern<RGBColor> pattern_;
};
*/
