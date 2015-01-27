#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <map>

namespace Deck
{

    static const unsigned int RankN = 13;
    enum Rank {Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace, UNKNOWN_RANK};
    inline Rank operator++(enum Rank& rank) { return rank = static_cast<Rank>(static_cast<int>(rank) + 1); }
    static const char Rank2Abr[14] = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A', 'U' };
    static const char* Rank2Name[14] = { "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace", "UNKNOWN_RANK" };
    class RankMap
    {
        Rank map_[0x100];
    public:
        RankMap(void)
        {
            for (int i = 0; i < 0x100; ++i)
                map_[i] = UNKNOWN_RANK;
            for (Rank i = Two; i <= Ace; ++i)
                map_[static_cast<const unsigned char>(Rank2Abr[i])] = i;
        }
        Rank operator[](const char c) const { return map_[static_cast<const unsigned char>(c)]; }
        friend std::ostream& operator<<(std::ostream& stream, const RankMap& rank_map);
    };
    inline std::ostream& operator<<(std::ostream& stream, const RankMap& rank_map)
    {
        for (int i = 0; i < 0x100; ++i)
            stream << rank_map.map_[i];
        return stream;
    }
    inline std::ostream& operator<<(std::ostream& stream, const Rank& rank) { return stream << Rank2Name[rank]; }

    static const unsigned int SuitN = 4;
    enum Suit {Hearts, Diamonds, Clubs, Spades, UNKNOWN_SUIT};
    inline Suit operator++(enum Suit& suit) { return suit = static_cast<Suit>(static_cast<int>(suit) + 1); }
    static const char Suit2Abr[5] = { 'h', 'd', 'c', 's', 'u' };
    static const char* Suit2Name[5] = { "Hearts", "Diamonds", "Clubs", "Spades", "UNKNOWN_SUIT" };
    static std::map<char, Rank> AbrRank;
    class SuitMap
    {
        Suit map_[0x100];
    public:
        SuitMap(void)
        {
            for (int i = 0; i < 0x100; ++i)
                map_[i] = UNKNOWN_SUIT;
            for (Suit i = Hearts; i <= Spades; ++i)
                map_[static_cast<const unsigned char>(Suit2Abr[i])] = i;
        }
        Suit operator[](const char c) const { return map_[static_cast<const unsigned char>(c)]; }
    };
    inline std::ostream& operator<<(std::ostream& stream, const enum Suit suit) { return stream << Suit2Name[suit]; }

    struct Card
    {
        Card(Rank r = UNKNOWN_RANK, Suit s = UNKNOWN_SUIT): rank(r), suit(s) { }
        bool operator==(const struct Card& a) { return (suit == a.suit && rank == a.rank); }
        bool operator!=(const struct Card& a) { return !(*this == a); }
        Rank rank;
        Suit suit;
    };
    inline std::ostream& operator<<(std::ostream& stream, const struct Card& card) { return stream << card.rank << " of " << card.suit; }

}

#endif
