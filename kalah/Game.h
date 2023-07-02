#ifndef _GAME_
#define _GAME_
#include "Board.h"
class Player;

class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side s);
    void play();
    int beans(Side s, int hole) const;



private:
    Board m_board;
    Player* m_south;
    Player* m_north;
    Side turn;
    
};


#endif
