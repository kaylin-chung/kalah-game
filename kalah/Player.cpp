#include "Player.h"
#include <climits>
#include <iostream>
#include <chrono>
#include <random>
using namespace std;

Player::Player(string name)
  :m_name(name)
{
    
}

std::string Player::name() const
{
    return m_name;
}

bool Player::isInteractive() const
{
    return false;
}

Player::~Player()
{
    
}

HumanPlayer::HumanPlayer(string name)
  :Player(name)
{
    
}

bool HumanPlayer::isInteractive() const
{
    return true;
}

int HumanPlayer::chooseMove(const Board &b, Side s) const
{
    int choice;
    do {
        cout << "Select a hole, " + name() + ": ";
        cin >> choice;
        if (choice <= 0 || b.holes() < choice)
        {
            cout << "Invalid hole! Please select between 1 and " << to_string(b.holes()) << endl;
        }
        else if (b.beans(s, choice) == 0)
        {
            cout << "There are no beans in that hole!" << endl;
        }
            
    } while (choice <= 0 || choice > b.holes() || b.beans(s,choice) == 0);
    return choice;
    return 0;
}



BadPlayer::BadPlayer(std::string name)
  :Player(name)
{ }

int BadPlayer::chooseMove(const Board &b, Side s) const
{
    if (b.beansInPlay(s) == 0)
    {
        return -1;
    }
    
    default_random_engine generator;
    uniform_int_distribution<int> distribution(1,b.holes());
    int badMove;
    do {
        badMove = distribution(generator);
    } while (badMove <= 0 || badMove > b.holes() || b.beans(s,badMove) == 0);
    
    return badMove;
}


SmartPlayer::SmartPlayer(string name)
  :Player(name)
{
    
}

int SmartPlayer::chooseMove(const Board &b, Side s) const
{
    int bestMove, value, depth(0);
    JumpyTimer timer(1000);
    double timeLimit = 4990;
    chooseMove(b, s, bestMove, value, depth, timer, timeLimit);
    return bestMove;
}

bool SmartPlayer::completeMove(Board &b, Side s, int hole, Side& endSide, int& endHole) const
{
    if (b.sow(s, hole, endSide, endHole)) 
    {
        if (endHole == POT)
        {
            return false;
        }
        if (endSide == s)
        {
            if (b.beans(s,endHole) == 1 && b.beans(opponent(s), endHole) != 0)
            {
                b.moveToPot(opponent(s), endHole, s);
                b.moveToPot(s, endHole, s);
            }
        }
        return true;
    }
    return false;
}

void SmartPlayer::chooseMove(const Board& b, Side s, int& bestMove, int& value, int depth, JumpyTimer timer, double timeLimit) const
{
    int limit = 6;
    
    if (b.beansInPlay(s) == 0) // gameover
    {
        bestMove = -1;
        value = evaluate(b);
        return;
    }
    
    if (timer.actualElapsed() > timeLimit) // exceeds time limit
    {
        bestMove = -1;
        value = evaluate(b);
        return;
    }

    if (depth > limit) // exceeds depth limit
    {
        bestMove = -1;
        value = evaluate(b);
        return;
    }
    int firstHole = 1;
    for (int i=1; i<b.holes(); i++)
    {
        if (b.beans(s, i) == 0) // can't make move so advance hole (ensures that no illegal moves are made)
        {
            firstHole++;
            continue;
        }
        Board temp(b);
        Side endSide;
        int endHole, v2, h2, h3;
        if (completeMove(temp, s, i, endSide, endHole)) // if s completes its move
        {
            chooseMove(temp, opponent(s), h2, v2, depth+1, timer, timeLimit);
        }
        else
        {
            chooseMove(temp, s, h3, v2, depth, timer, timeLimit);
        }
        if (i == firstHole) // if firstHole was the best hole
        {
            bestMove = i;
            value = v2;
        }
        if (s == SOUTH) // wants to MAXIMIZE
        {
            if (v2 > value)
            {
                value = v2;
                bestMove = i;
            }
        }
        else // NORTH wants to MINIMIZE
        {
            if (v2 < value)
            {
                value = v2;
                bestMove = i;
            }
        }
    }
    return;

}

// positive good for south
// negative good for north
int SmartPlayer::evaluate(const Board &b) const
{
    int north_beans = b.beansInPlay(NORTH);
    int south_beans = b.beansInPlay(SOUTH);
    int north_pot = b.beans(NORTH, 0);
    int south_pot = b.beans(SOUTH, 0);
    if (north_beans == 0 || south_beans == 0)
    {
        if (south_pot < north_pot) // south will lose
            return INT_MIN;
        else if (south_pot > north_pot) // north will lose
            return INT_MAX;
        else // it's a draw
            return 0;
    }
    return (south_beans + south_pot) - (north_beans + north_pot);
}


