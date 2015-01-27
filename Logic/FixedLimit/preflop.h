#include "../../deck.h"
#include <vector>

enum Actions {AC_FOLD, AC_CALL, AC_RAISE, AC_ALLIN};

class Action
{
	explicit Action(Actions type) {}
	virtual ~Action() {}
};


struct Situation
{
	std::vector<unsigned> stakes;
	std::vector<unsigned> bets;
};

struct PreflopState
{
	Deck::Card cards[2];
	unsigned int position;
};


enum Positions {PS_EARLY, PS_MIDLE, PS_LATE, PS_SMALL_B, PS_BIG_B, PS_UNKNOWN};

enum Power {PW_STRONG, PW_MEDICORE, PW_SPECULATIVE, PW_MIXED, PW_TRASH};

class PreflopChart
{
public:
	virtual ~PreflopChart() {}
	virtual Actions act(const PreflopState& state, const Situation& situation) = 0;	
};

class BasicChart : public PreflopChart
{
public:
	virtual Actions act(const PreflopState& state, const Situation& situation);
private:
	int calculate_state(const PreflopState& state);
	int calculate_actions(const Situation& situation);
	static const Actions strong[5][5];
	static const Actions mediocre[5][5];
	static const Actions speculative[5][5];
	static const Actions mixed[5][5];
};
