#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

void doBoardTests()
{
    Board invalidBoard(-1,-1);
    assert(invalidBoard.holes() == 1  &&  invalidBoard.totalBeans() == 0  &&
           invalidBoard.beans(SOUTH, POT) == 0  &&  invalidBoard.beansInPlay(SOUTH) == 0);

    Board b(3, 2);
    assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
                    b.beans(SOUTH, POT) == 0  &&  b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);
    assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
                b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, POT) == 2  &&
        b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
                b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
        b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    
    Board b1(5,3);
    Side es1;
    int eh1;
    b1.sow(NORTH, 2, es1, eh1);
    assert(es1 == SOUTH  &&  eh1 == 1 && b1.beans(NORTH, 2) == 0
           && b1.beans(NORTH, 1 == 4) && b1.beans(SOUTH,1) == 4);
    b1.setBeans(NORTH, 1, 8);
    assert(b1.totalBeans() == 34);
    b1.sow(NORTH, 1, es1, eh1);
    assert(es1 == NORTH  &&  eh1 == 4 && b1.beans(SOUTH, 0) == 0);
    b1.moveToPot(SOUTH, 2, NORTH);
    assert(b1.beans(NORTH, 0) == 6);
    assert(!b1.moveToPot(SOUTH, 0, SOUTH));
    assert(!b1.moveToPot(SOUTH, 7, NORTH));
    b1.sow(SOUTH, 3, es1, eh1);
    assert(es1 == NORTH && eh1 == 5 && b1.beans(SOUTH, 4) == 5
           && b1.beans(SOUTH, 5) == 5 && b1.beans(SOUTH, 0) == 1
            && b1.beans(NORTH, 5 == 1));
}

void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board x(3, 2);
    x.setBeans(SOUTH, 2, 0);
    cout << "=========" << endl;
    int n = hp.chooseMove(x, SOUTH);
    cout << "=========" << endl;
    assert(n == 1  ||  n == 3);
    n = bp.chooseMove(x, SOUTH);
    assert(n == 1  ||  n == 3);
    n = sp.chooseMove(x, SOUTH);
    assert(n == 1  ||  n == 3);
    x.setBeans(SOUTH, 1, 0);
    x.setBeans(SOUTH, 3, 0);
    assert(bp.chooseMove(x, SOUTH) == -1);
    assert(sp.chooseMove(x, SOUTH) == -1);
}


void doGameTests()
{
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
      //    Homer
      //   0  1  2
      // 0         0
      //   2  0  0
      //    Bart
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
    g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
      //   0  1  0
      // 0         3
      //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(NORTH);
      //   1  0  0
      // 0         3
      //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
      //   1  0  0
      // 0         3
      //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

    g.move(NORTH); // tests the sweeping implementation
      //   0  0  0
      // 1         4
      //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
    
    
    BadPlayer bp3("Mickey");
    BadPlayer bp4("Minnie");
    Board b2(5, 2);
    Game g2(b2, &bp3, &bp4);
    g2.play(); //my bad player makes random moves, tests that move(s) can take multiple turns and perform captures

}
void sampleGame()
{
    SmartPlayer bp1("Smallberg");
    BadPlayer bp2("Nachenberg");
    Board b(6, 4);
    
    Game g(b, &bp1, &bp2);
    g.play();
    //  Nachenberg
   // 1  2  3  4  5  6
 //   ---------------
  // 4  4  4  4  4  4
//  0                0
  // 4  4  4  4  4  4
 //   ---------------
   // 1  2  3  4  5  6
  //     Smallberg
    bool over, hasWinner;
    Side winner;
    g.status(over, hasWinner, winner);
    assert(winner == SOUTH);
    
    BadPlayer bp3("Tom");
    SmartPlayer bp4("Jerry");
    Board b2(6, 4);
    
    Game g2(b2, &bp3, &bp4);
    g2.play();
    //  Jerry
   // 1  2  3  4  5  6
 //   ---------------
  // 4  4  4  4  4  4
//  0                0
  // 4  4  4  4  4  4
 //   ---------------
   // 1  2  3  4  5  6
  //     Tom
    bool over2, hasWinner2;
    Side winner2;
    g2.status(over2, hasWinner2, winner2);
    assert(winner2 == NORTH);
}

int main()
{
    doBoardTests();
    doPlayerTests();
    doGameTests();
    sampleGame();
    
    cout << "Passed all tests" << endl;
}
