#include "interface.h"

/* returns the eval for a board */
int totalVal(Board board)
{
    int total;

    total = totalMaterial(board.allPieces);

    return total;
}

/* returns the material eval for a position */
int totalMaterial(Piece allPieces[8][8])
{
    int m, n, total = 0;

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            if (allPieces[m][n].colour) {
                total += allPieces[m][n].typeVal;
            } else {
                total -= allPieces[m][n].typeVal;
            }

        }
    }

    return total;
}
