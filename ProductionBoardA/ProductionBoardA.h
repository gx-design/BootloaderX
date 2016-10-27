#ifndef PRODUCTION_BOARD_A_H_
#define PRODUCTION_BOARD_A_H_

#include "IBoard.h"

class ProductionBoardA : public IBoard
{
  public:
    ProductionBoardA ();

    void Initialise ();
    static void DisableInterrupts ();
    static void EnableInterrupts ();

    bool ForceBootloadRequested ();
};

#endif
PRODUCTION_BOARD_A_H_
