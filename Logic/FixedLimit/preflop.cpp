#include "Preflop.h"

#include <exception>
#include <stdexcept>

#define NO_IMPLEMENTATION throw std::runtime_error("No implementation"); 

Positions positions_easy_converter(unsigned int position)
{
	if (position < 1) return PS_SMALL_B;
	if (position < 2) return PS_BIG_B;
	if (position < 5) return PS_EARLY;
	if (position < 8) return PS_MIDLE;
	if (position < 11) return PS_LATE;
	return PS_UNKNOWN;
}



class Table
{
public:
	// максимальное число игроков за столом
	unsigned int NMax() const {return n_max;} 
	// число игроков за столом
	unsigned int Number() const {return count;} 
private:
	unsigned int count;
	unsigned int n_max;
};



Actions BasicChart::act(const PreflopState& state, const Situation& situation)
{
	/// first circle
	// very strong
	if( ((state.cards[0].rank >= Deck::King) && (state.cards[1].rank >= Deck::King)) ||
		((state.cards[0].rank == Deck::Queen) && (state.cards[1].rank == Deck::Queen))
		)
	{
		return AC_RAISE;
	}
	int res = calculate_state(state);
	Positions pos = positions_easy_converter(state.position);
	int act = calculate_actions(situation);
	if (act > 4)
	{
		return AC_FOLD;
	}
	if (pos > 4 || pos < 0)
	{
		NO_IMPLEMENTATION
	}
	switch(res)
	{
	case PW_STRONG:
		return strong[act][pos];
	case PW_MEDICORE:
		return mediocre[act][pos];
	case PW_SPECULATIVE:
		return speculative[act][pos];
	case PW_MIXED:
		return mixed[act][pos];
	default:
		return AC_FOLD;
	}
	// second circle
	// NO_IMPLEMENTATION;
}

int BasicChart::calculate_state(const PreflopState& state)
{
	if( (state.cards[0].rank >= Deck::Nine) )
	{
		if (state.cards[1].rank == state.cards[0].rank)
		{
			return PW_STRONG;
		}
	}
	Deck::Card big_card = state.cards[0];
	Deck::Card small_card = state.cards[1];
	if (state.cards[0].rank < state.cards[1].rank)
	{
		std::swap(big_card, small_card);
	}
	if (big_card.rank == Deck::Ace)
	{
		if (small_card.rank == Deck::Queen)
		{
			return PW_STRONG;
		}

		if (small_card.rank == Deck::Jack && small_card.suit == big_card.suit)
		{
			return PW_STRONG;
		}
		if (small_card.rank >= Deck::Ten)
		{
			return PW_MEDICORE;
		}
	}
	if (big_card.rank == Deck::King)
	{
		if (big_card.rank == Deck::Queen)
		{
			return PW_MEDICORE;
		}
	}

	if (big_card.rank == small_card.rank)
	{
		return PW_SPECULATIVE;
	}
	if (big_card.suit == small_card.suit)
	{
		if (small_card.rank >= Deck::Ten)
		{
			return PW_SPECULATIVE;
		}
		if (big_card.rank == Deck::Ten && small_card.rank == Deck::Nine)
		{
			return PW_SPECULATIVE;
		}
		if (big_card.rank == Deck::Ace)
		{
			return PW_MIXED;
		}
		if (big_card.rank == Deck::King)
		{
			if (small_card.rank >= Deck::Nine)
			{
				return PW_MIXED;
			}
			if (small_card.rank >= Deck::Seven)
			{
				if (big_card.rank - small_card.rank == 1)
				{
					return PW_MIXED;
				}
			}
		}
	}
	if (small_card.rank >= Deck::Ten)
	{
		return PW_MIXED;
	}
	return PW_TRASH;
}

int BasicChart::calculate_actions(const Situation& situation)
{
	int num_of_limpers = 0;
	// int num_of_folds = 0;
	int num_of_rasers = 0;
	int num_of_callers = 0;
	unsigned int last_bet = situation.bets[1];
	for (unsigned int i = 2; i < situation.bets.size(); ++i)
	{
		if (situation.bets[i] == situation.bets[1])
		{
			++num_of_limpers;
		}
		if (situation.bets[i] > situation.bets[1])
		{
			if (situation.bets[i] > last_bet)
			{
				last_bet = situation.bets[i];
				++num_of_rasers;
			}
			else
			{
				++num_of_callers;
			}
		}
	}
	if (num_of_rasers  == 0)
	{
		if (num_of_limpers == 0) return 0;
		if (num_of_limpers == 1) return 1;
		if (num_of_limpers > 1 ) return 2;
	}
	if (num_of_rasers == 1 )
	{
		if (num_of_callers == 0) return 3;
		if (num_of_callers > 0) return 4;
	}
	if (num_of_rasers > 1)
	{
		return 5;
	}
	// second circle
	NO_IMPLEMENTATION
}

const Actions BasicChart::strong[5][5] = 
{
	{AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_FOLD, AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_CALL, AC_CALL, AC_CALL, AC_CALL, AC_CALL}
};

const Actions BasicChart::mediocre[5][5] = 
{
	{AC_FOLD, AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_FOLD, AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_FOLD, AC_RAISE, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_FOLD, AC_FOLD, AC_FOLD, AC_FOLD, AC_CALL},
	{AC_FOLD, AC_FOLD, AC_FOLD, AC_FOLD, AC_CALL},
};

const Actions BasicChart::speculative[5][5] = 
{
	{AC_FOLD, AC_FOLD, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_FOLD, AC_FOLD, AC_CALL, AC_CALL, AC_CALL},
	{AC_CALL, AC_CALL, AC_CALL, AC_CALL, AC_CALL},
	{AC_FOLD, AC_FOLD, AC_FOLD, AC_FOLD, AC_CALL},
	{AC_CALL, AC_CALL, AC_CALL, AC_CALL, AC_CALL},
};

const Actions BasicChart::mixed[5][5] = 
{
	{AC_FOLD, AC_FOLD, AC_RAISE, AC_RAISE, AC_RAISE},
	{AC_FOLD, AC_FOLD, AC_FOLD, AC_CALL, AC_CALL},
	{AC_FOLD, AC_FOLD, AC_CALL, AC_CALL, AC_CALL},
	{AC_FOLD, AC_FOLD, AC_FOLD, AC_FOLD, AC_FOLD},
	{AC_FOLD, AC_FOLD, AC_FOLD, AC_FOLD, AC_FOLD},
};


class Preflop
{

};
