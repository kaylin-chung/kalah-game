#ifndef _PLAYER_
#define _PLAYER_
#include <string>
#include "Board.h"
#include "Side.h"

class Player
{
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
    
private:
    std::string m_name;
};


class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    
};

class BadPlayer : public Player
{
public:
    BadPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
    
};

class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
private:
    bool completeMove(Board &b, Side s, int hole, Side& endSide, int& endHole) const;
    void chooseMove(const Board& b, Side s, int& bestMove, int& value, int depth, JumpyTimer timer, double timeLimit) const;
    int evaluate(const Board &b) const;
    
};


#endif
