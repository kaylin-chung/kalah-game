#ifndef _BOARD_
#define _BOARD_
#include "Side.h"
#include <vector>

class Board
{
public:
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
    
private:
    int m_holes;
    int m_totalbeans;
    std::vector <int> north_side;
    std::vector <int> south_side;
    void initializeSide(int beans, std::vector<int> &m_side);
    bool isValidHole(int hole) const;
    bool isValidSide(Side s) const;
};


#endif 
