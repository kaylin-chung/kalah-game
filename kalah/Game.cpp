#include "Game.h"
#include "Player.h"
#include <iostream>
using namespace std;
Game::Game(const Board& b, Player* south, Player* north)
 : m_board(b), m_south(south), m_north(north), turn(SOUTH)
{
    
}
void Game::display() const
{
    cout << "\t" + m_north->name() << endl;
    cout << "  ";
    for (int i=1; i<=m_board.holes(); i++)
    {
        cout << " " << i << " ";
    }
    cout << endl << " " << "---------------" << endl;
    cout << "  ";
    for (int i=1; i<=m_board.holes(); i++)
    {
        cout << " " + to_string(m_board.beans(NORTH, i)) + " ";
    }
    cout << endl;
    cout << m_board.beans(NORTH, POT) << "                " << m_board.beans(SOUTH, POT) <<endl;
    cout << "  ";
    for (int i=1; i<=m_board.holes(); i++)
    {
        cout << " " + to_string(m_board.beans(SOUTH, i)) + " ";
    }
    cout << endl << " " << "---------------" << endl;
    cout << "  ";
    for (int i=1; i<=m_board.holes(); i++)
    {
        cout << " " << i << " ";
    }
    cout << endl;
    cout << "\t" + m_south->name() << endl;
    
    
}
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    if (m_board.beansInPlay(NORTH) != 0 || m_board.beansInPlay(SOUTH) != 0)
    {
        over = false;
        hasWinner = false;
        return;
    }
    over = true;
    if (m_board.beans(NORTH, POT) > m_board.beans(SOUTH, POT))
    {
        hasWinner = true;
        winner = NORTH;
    }
    else
    {
        if (m_board.beans(SOUTH, POT) > m_board.beans(NORTH, POT))
        {
            hasWinner = true;
            winner = SOUTH;
        }
        else
        {
            hasWinner = false;
        }
    }
    return;
}

bool Game::move(Side s)
{
    int endHole;
    Side endSide;
    if (s == SOUTH)
    {
        int hole = m_south->chooseMove(m_board, SOUTH);
        if (!m_south->isInteractive())
        {
            if (m_board.beansInPlay(SOUTH) == 0)
            {
                cout << m_south->name() + ", there are no more beans left."<< endl;
            }
            else
            {
                cout << m_south->name() + " chooses hole " + to_string(hole) << endl;
                display();
            }
        }
        if (m_board.sow(SOUTH, hole, endSide, endHole))
        {
            if (endSide == SOUTH && endHole == POT)
            {
                display();
                if (m_board.beansInPlay(SOUTH)>0)
                {
                    cout << m_south->name() << " gets another turn" << endl;
                    return move(SOUTH); //gets another turn
                }
            }
            else if (endSide == SOUTH)
            {
                if (m_board.beans(SOUTH, endHole) == 1 && m_board.beans(NORTH, endHole) != 0) // performs a capture
                {
                    cout << "Captured!" << endl;
                    m_board.moveToPot(NORTH, endHole, SOUTH);
                    m_board.moveToPot(SOUTH, endHole, SOUTH);
                }
            }
        }
    }
    if (s == NORTH)
    {
        int hole = m_north->chooseMove(m_board, NORTH);
        if (!m_north->isInteractive())
        {
            if (m_board.beansInPlay(NORTH) == 0)
            {
                cout << m_south->name() + ", there are no more beans left." << endl;
            }
            else
            {
                cout << m_south->name() + " chooses hole " + to_string(hole) << endl;
                display();
            }
        }
        if (m_board.sow(NORTH, hole, endSide, endHole))
        {
            if (endSide == NORTH && endHole == POT)
            {
                display();
                if (m_board.beansInPlay(NORTH)>0)
                {
                    cout << m_north->name() << " gets another turn" << endl;
                    return move(NORTH); //gets another turn
                }
            }
            else if (endSide == NORTH)
            {
                if (m_board.beans(NORTH, endHole) == 1 && m_board.beans(SOUTH, endHole) != 0) // performs a capture
                {
                    m_board.moveToPot(SOUTH, endHole, NORTH);
                    m_board.moveToPot(NORTH, endHole, NORTH);
                }
            }
        }
    }
    if (m_board.beansInPlay(SOUTH) == 0)
    {
        for (int i=1; i<=m_board.holes(); i++) // sweeps NORTH's remaining beans to NORTH pot
        {
            m_board.moveToPot(NORTH, i, NORTH);
        }
        cout << "Sweeping " + m_north->name() + "'s beans...'" << endl;
        display();
        return false;
    }
    else if (m_board.beansInPlay(NORTH) == 0)
    {
        for (int i=1; i<=m_board.holes(); i++) // sweeps SOUTH's remaining beans to SOUTH pot
        {
            m_board.moveToPot(SOUTH, i, SOUTH);
        }
        cout << "Sweeping " + m_south->name() + "'s beans...'" << endl;
        display();
        return false;
    }
    return true;
}

void Game::play()
{
    bool over, hasWinner;
    Side winner;
    do
    {
        display();
        move(turn);
        if (!m_south->isInteractive() && !m_north->isInteractive())
        {
            cout << "Press ENTER to continue" << endl;
            cin.ignore(10000, '\n');
        }
        move(opponent(turn));
        status(over, hasWinner, winner);
        
        
    } while (!over);
    if (hasWinner)
    {
        if (winner == SOUTH)
        {
            cout << m_south->name() << " wins!" << endl;
        }
        else
        {
            cout << m_north->name() << " wins!" << endl;
        }
    }
    else
    {
        cout << "Tie!" << endl;
    }
}

int Game::beans(Side s, int hole) const
{
    return m_board.beans(s, hole);
}
