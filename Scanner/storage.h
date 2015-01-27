#ifndef STORAGE_H
#define STORAGE_H

#include "pattern.h"
#include "../deck.h"

#include <fstream>
#include <boost/filesystem.hpp>

struct Layout
{
    Layout(const std::string& filename);
    unsigned int width, height;
    struct {
        unsigned int w, h;
        unsigned int suit_y_shift;
        struct { unsigned int x, y; } pos[5];
    } widow;
    struct {
        unsigned int w, h;
        struct { unsigned int x, y; } pos[6];
    } button6;
};

class Storage
{
public:
    Storage(const std::string& layout_filename, const std::string& directory);

    void learnCard(const Image& image, const struct Deck::Card card);
    Deck::Card recognizeCard(const Image& image) const;
    unsigned int findButton(const Image& image, unsigned int seats_n) const;

    void test__(const std::string& layout_filename, const boost::filesystem::path& directory);

private:
    Layout layout_;
    Pattern<MonoColor> ranks_[Deck::RankN];
    Pattern<RGBColor> suits_[Deck::SuitN];
    Pattern<RGBColor> button_, no_button_;
};

#endif
