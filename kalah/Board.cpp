#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
{
    if (nHoles <= 0)
        nHoles = 1;
    m_holes = nHoles + 1; // add 1 for the pot
    if (nInitialBeansPerHole <= 0)
        nInitialBeansPerHole = 0;
    m_totalbeans = 2*(m_holes-1)*nInitialBeansPerHole;
    initializeSide(nInitialBeansPerHole, north_side);
    initializeSide(nInitialBeansPerHole, south_side);
    
}

int Board::holes() const
{
    return m_holes - 1; //not including pot
}

int Board::beans(Side s, int hole) const
{
    if (!isValidHole(hole) || !isValidSide(s))
        return -1;
    else
        if (s == SOUTH)
            return south_side[hole];
        else //NORTH
            return north_side[hole];
}

int Board::beansInPlay(Side s) const
{
    if (!isValidSide(s))
        return -1;
    int total = 0;
    if (s==SOUTH)
        for (int i=1; i<m_holes; i++)
            total += south_side[i];
    else
        for (int i=1; i<m_holes; i++)
            total += north_side[i];
    return total;
}

int Board::totalBeans() const
{
    return m_totalbeans;
}
// SCENARIOS
// the current hole is 0 meaning it goes past the pot (switch sides, current hole is the last hole)
// the current hole is one before the pot (currentHole is 0, increment beans in that pot)
// the current hole is not before the pot (increment currentHole, increment beans in that pot)
// the current hole is on the opp side and is not right before the pot (side doesn't change, )
// the current hole is on the opp side and is right before the pot (side changes, increment beans in the opp pot)
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if (!isValidSide(s) || !isValidHole(hole))
        return false;
    if (hole == POT)
        return false;
    switch (s)
    {
        case(NORTH):
            if (north_side[hole] == 0)
                return false;
            break;
        case(SOUTH):
            if (south_side[hole] == 0)
                return false;
            break;
    }
    
    int currentHole = hole;
    Side currentSide = s;
    
    if (s == SOUTH)
    {
        int counter = south_side[hole]; // number of beans to start
        while (counter>0)
        {
            if (currentHole == 0) //switch sides since it goes pass the pot
            {
                currentSide = NORTH;
                currentHole = m_holes;
            }
            if (currentSide == SOUTH)
            {
                if (currentHole < m_holes - 1) // current hole is not the hole right before the pot
                {
                    currentHole++;
                    south_side[currentHole]++;
                }
                else //next hole is the pot
                {
                    currentHole = 0;
                    south_side[currentHole]++;
                }
            }
            if (currentSide == NORTH)
            {
                if (currentHole > 1) // currentHole is not the hole right before the pot
                {
                    currentHole--;
                    north_side[currentHole]++;
                }
                else // the next hole is the pot so we skip it and switch sides
                {
                    currentSide = SOUTH;
                    south_side[currentHole]++; // will be SOUTH's first hole
                }
            }
            south_side[hole]--; //pick up bean from original hole each iteration
            counter--;
        }
    }
    else // NORTH side
    {
        int counter = north_side[hole]; // number of beans to start
        while (counter>0)
        {
            if (currentHole == 0) //switch sides if it goes pass the pot
            {
                currentSide = SOUTH;
            }
            if (currentSide == SOUTH)
            {
                if (currentHole < m_holes - 1) // not right before SOUTH pot
                {
                    currentHole++;
                    south_side[currentHole]++;
                }
                else // right before SOUTH pot
                {
                    currentSide = NORTH; // skip south pot and switch sides 
                    north_side[currentHole]++; // will be the last one
                    
                }
            }
            else // currentSide is NORTH
            {
                if (currentHole > 0) // if the currentHole is not the hole right before the pot
                {
                    currentHole--;
                    north_side[currentHole]++;
                }
            }
            north_side[hole]--;
            counter--;
        }
    }
    if (currentHole == 0 || currentHole == m_holes) // checks if the current hole is a pot
    {
        currentHole = POT;
    }
    endHole = currentHole;
    endSide = currentSide;
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if (!isValidHole(hole) || !isValidSide(s))
        return false;
    if (hole == POT)
        return false;
    int beans_to_move;
    if (s==SOUTH)
    {
        beans_to_move = south_side[hole];
        if (potOwner == SOUTH)
        {
            south_side[POT] += beans_to_move;
        }
        if (potOwner == NORTH)
        {
            north_side[POT] += beans_to_move;
        }
        south_side[hole] = 0; //empties that hole
    }
    else // NORTH SIDE
    {
        beans_to_move = north_side[hole];
        if (potOwner == SOUTH)
        {
            south_side[POT] += beans_to_move;
        }
        if (potOwner == NORTH)
        {
            north_side[POT] += beans_to_move;
        }
        north_side[hole] = 0; //empties that hole
    }
    return true;
}

bool Board::setBeans(Side s, int hole, int beans)
{
    if (!isValidSide(s) || !isValidHole(hole) || beans < 0)
        return false;
    
    int previous_beans;
    
    if (s == SOUTH)
    {
        previous_beans = south_side[hole];
        south_side[hole] = beans;
    }
    else
    {
        previous_beans = north_side[hole];
        north_side[hole] = beans;
    }
    m_totalbeans = m_totalbeans - previous_beans + beans; //updates the total amount of beans
    return true;
}

void Board::initializeSide(int beans, std::vector<int> &m_side)
{
    m_side.push_back(0); //sets pot to 0
    for (int i=1; i<m_holes; i++)
        m_side.push_back(beans);
}

bool Board::isValidHole(int hole) const
{
    for (int i=0; i<m_holes; i++)
    {
        if (hole == i)
            return true;
    }
    return false;
}

bool Board::isValidSide(Side s) const
{
    switch (s)
    {
        case(NORTH):
        case(SOUTH):
            return true;
        default:
            return false;
    }
}
