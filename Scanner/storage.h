#ifndef STORAGE_H
#define STORAGE_H

#include "pattern.h"
#include "../deck.h"

#include <fstream>
#include <vector>
#include <string>


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
    Storage(const std::string& layout_filename, const std::string& directory, bool learnFast = false);

	void setLayout(const std::string& layout_filename);
    void learnCard(const Image& image, const Deck::Card& card);
    Deck::Card recognizeCard(const Image& image) const;
    unsigned int findButton(const Image& image, unsigned int seats_n) const;
	// �������� �� Image&
	// std::vector<Deck::Card> recognizeTuple(const BMP& bmp) const;
	std::vector<Deck::Card> recognizeTupleFromImage(const Image& bmp) const;
	std::string serializeTuple(const std::vector<Deck::Card>& vec) const;
	std::string getStringFromBmp(const Image& bmp) const;
	bool hasUnlearnedCards() const;
    void test__(const std::string& layout_filename, const std::string& directory);

private:
    Layout layout_;
    Pattern<MonoColor> ranks_[Deck::RankN];
    Pattern<RGBColor> suits_[Deck::SuitN];
    Pattern<RGBColor> button_, no_button_;
	bool known_ranks[Deck::RankN];
	bool known_suits[Deck::SuitN];
};

#endif
