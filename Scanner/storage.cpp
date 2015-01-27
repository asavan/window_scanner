#include "storage.h"

#include <iostream>
#include <string>
// #include <limits>

namespace Deck
{
    static RankMap Abr2Rank;
    static SuitMap Abr2Suit;
};

Layout::Layout(const std::string& filename)
{
    std::fstream file(filename.c_str());
    if (!file.is_open())
        throw std::invalid_argument(filename + " is missing");
    std::string junk;

    std::getline(file, junk);
    std::getline(file, junk);
    std::getline(file, junk);
    file >> width >> height;
    getline(file, junk);
    file >> widow.w >> widow.h;
    getline(file, junk);
    file >> widow.suit_y_shift;
    getline(file, junk);
    for (int i = 0; i < 5; ++i)
    {
        file >> widow.pos[i].x >> widow.pos[i].y;
        getline(file, junk);
    }
    file >> button6.w >> button6.h;
    getline(file, junk);
    for (int i = 0; i < 6; ++i)
    {
        file >> button6.pos[i].x >> button6.pos[i].y;
        getline(file, junk);
    }

}

std::ostream& operator<<(std::ostream& stream, const Layout& l)
{
    stream << l.width << " " <<  l.height << std::endl;
    stream << l.widow.w << " " <<  l.widow.h << std::endl;
    stream << l.widow.suit_y_shift << std::endl;
    for (int i = 0; i < 5; ++i)
        stream << l.widow.pos[i].x << " " << l.widow.pos[i].y << std::endl;
    return stream;
}

Storage::Storage(const std::string& layout_filename, const std::string& base_dir1): layout_(layout_filename)
{
	boost::filesystem::path base_dir(base_dir1);
    for (boost::filesystem::directory_iterator iter(base_dir), end; iter != end; ++iter )
    {
		std::string filename = iter->path().filename().string();
        if (is_directory(*iter) || filename.size() < 10 || filename.size()%2 || filename.compare(filename.size() - 4, 4, ".bmp"))
            continue;

		std::string full_name = base_dir.string() + '/' + filename;
        std::fstream file(full_name.c_str(), std::ios::in|std::ios::binary);
		boost::uintmax_t filesize = boost::filesystem::file_size(full_name);
        char*const bmp_image = new char [filesize];
        file.read(bmp_image, filesize);
        file.close();

        
        std::string::const_iterator it = filename.begin();
        for (int i = 0; *it != '.' && *it != 'B'; ++i, ++it)
        {
            Deck::Rank rank = Deck::Abr2Rank[*it];
            Deck::Suit suit = Deck::Abr2Suit[*(++it)];
            learnCard(Image(BMP(reinterpret_cast<const unsigned char*>(bmp_image)), layout_.widow.pos[i].x, layout_.widow.pos[i].y, layout_.widow.w, layout_.widow.h + layout_.widow.suit_y_shift), Deck::Card(rank, suit));
        }
        if (*it == 'B')
		{
            for (int i = 0; i < 6; ++i)
			{
                if (i == *++it - '0')
				{
                    button_.merge(Image(BMP(reinterpret_cast<const unsigned char*>(bmp_image)), layout_.button6.pos[i].x, layout_.button6.pos[i].y, layout_.button6.w, layout_.button6.h));
				}
                else
				{
					no_button_.merge(Image(BMP(reinterpret_cast<const unsigned char*>(bmp_image)), layout_.button6.pos[i].x, layout_.button6.pos[i].y, layout_.button6.w, layout_.button6.h));
				}
			}
		}
		delete [] bmp_image;
    }
}

unsigned int Storage::findButton(const Image& image, unsigned int seats_n) const
{
    for (unsigned int i = 0; i < seats_n; ++i)
    {
		Pattern<RGBColor> pattern(image.clip(layout_.button6.pos[i].x, layout_.button6.pos[i].y, layout_.button6.w, layout_.button6.h));
        int64_t button_corr = Pattern<RGBColor>::corellation(button_, pattern, LinearCorellation); 
        int64_t no_button_corr = Pattern<RGBColor>::corellation(no_button_, pattern, LinearCorellation); 
        if (button_corr > no_button_corr)
            return i;
    }
    return seats_n+1;
}

void Storage::learnCard(const Image& image, const struct Deck::Card card)
{
    if (card.rank == Deck::UNKNOWN_RANK || card.suit == Deck::UNKNOWN_SUIT)
        throw std::invalid_argument("can't learn unknown card");
    Pattern<MonoColor> rank_pattern(image.clip(0, 0, layout_.widow.w, layout_.widow.h));
    ranks_[card.rank].merge(rank_pattern);
    Pattern<RGBColor> suit_pattern(image.clip(0, layout_.widow.suit_y_shift, layout_.widow.w, layout_.widow.h));
    suits_[card.suit].merge(suit_pattern);
    ranks_[card.rank].dump__("tmp", std::string("_base_") + Deck::Rank2Name[card.rank]);
    suits_[card.suit].dump__("tmp", std::string("_base_") + Deck::Suit2Name[card.suit]);
}

Deck::Card Storage::recognizeCard(const class Image& image) const
{
    const Pattern<MonoColor> rank_pattern(image.clip(0, 0, layout_.widow.w, layout_.widow.h));
    const Pattern<RGBColor> suit_pattern(image.clip(0, layout_.widow.suit_y_shift, layout_.widow.w, layout_.widow.h));
    rank_pattern.dump__("tmp", "RANK");
    suit_pattern.dump__("tmp", "SUIT");
    Deck::Rank best_rank = Deck::UNKNOWN_RANK;
    Deck::Suit best_suit = Deck::UNKNOWN_SUIT;
    Deck::Card best_card;

    for (CorellationType type = LinearCorellation; type <= LinearCorellation; ++type)
    {
        int64_t max_correlation = std::numeric_limits<int64_t>::min();
        for (Deck::Rank rank = Deck::Two; rank <= Deck::Ace; ++rank)
        {
            int64_t current_corellation = Pattern<MonoColor>::corellation(ranks_[rank], rank_pattern, type); 
            if (current_corellation > max_correlation)
            {
                max_correlation = current_corellation;
                best_rank = rank;
            }
        }
        max_correlation = std::numeric_limits<int64_t>::min();
        for (Deck::Suit suit = Deck::Hearts; suit <= Deck::Spades; ++suit)
        {
            int64_t current_corellation = Pattern<RGBColor>::corellation(suits_[suit], suit_pattern, type); 
            if (current_corellation > max_correlation)
            {
                max_correlation = current_corellation;
                best_suit = suit;
            }
        }

        best_card = Deck::Card(best_rank, best_suit);
    }

    return best_card;
}

std::vector<Deck::Card> Storage::recognizeTuple(const BMP& bmp) const
{
	std::vector<Deck::Card> results;
	for (unsigned int i = 0; i < 5; ++i)
    {
        Deck::Card card = recognizeCard(Image(bmp, layout_.widow.pos[i].x, layout_.widow.pos[i].y, layout_.widow.w, layout_.widow.h + layout_.widow.suit_y_shift));
		if (card.rank != Deck::Rank::UNKNOWN_RANK) 
		{
			results.push_back(card);
		}
		else
		{
			break;
		}
    }
	return results;
}

std::string Storage::serializeTuple(const std::vector<Deck::Card>& vec) const
{
	std::string str;
	for(std::vector<Deck::Card>::const_iterator it = vec.begin(); it != vec.end(); ++it) 
	{
		Deck::Card card = *it;
        str += Deck::Rank2Abr[card.rank];
        str += Deck::Suit2Abr[card.suit];
	}
	return str;
}

std::string Storage::getStringFromBmp(const BMP& bmp) const
{
	return serializeTuple(recognizeTuple(bmp));
}

void Storage::test__(const std::string& layout_file, const boost::filesystem::path& directory)
{
    layout_ = Layout(layout_file);

    for (boost::filesystem::directory_iterator iter(directory), end; iter != end; ++iter )
    {
		// std::string filename = iter->path().filename();
		std::string filename = iter->path().filename().string();
		
        if (is_directory(*iter) || filename.size() < 5 || filename.compare(filename.size() - 4, 4, ".bmp"))
            continue;

        std::fstream file((directory.string() + '/' + filename).c_str(), std::ios::in|std::ios::binary);
        char* bmp_image = new char [boost::filesystem::file_size((directory.string() + '/' + filename))];
        file.read(bmp_image, boost::filesystem::file_size((directory.string() + '/' + filename)));
        file.close();

        std::string new_filename;
        for (unsigned int i = 0; i < filename.find('.')/2; ++i)
        {
            Deck::Card card = recognizeCard(Image(BMP(reinterpret_cast<const unsigned char*>(bmp_image)), layout_.widow.pos[i].x, layout_.widow.pos[i].y, layout_.widow.w, layout_.widow.h + layout_.widow.suit_y_shift));
            new_filename += Deck::Rank2Abr[card.rank];
            new_filename += Deck::Suit2Abr[card.suit];
        }

		// нужно ли
		delete bmp_image;

//        int button_n = findButton(Image(reinterpret_cast<const unsigned char*>(bmp_image), 0, 0, layout_.width, layout_.height), 6);

        new_filename += ".bmp";
        if (filename != new_filename)
        {
            std::cout << filename << " != " << new_filename << ": ";
            std::cout << " FAILED" << std::endl;
        }
        else
        {
            std::cout << " PASSED" << std::endl;
        }
    }
}
