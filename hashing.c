#include "interface.h"

/* adds a position to the transtable */
int
addPosToTable(int colour, int depth, Board board, Action action)
{
    int index = genHashCode(board.allPieces);

    transTable[index].board = board;
    transTable[index].depth = depth;
    transTable[index].colour = colour;
    transTable[index].action = action;

    return 0;
}

/* generates an index to use based off of the board layout */
int
genHashCode(Piece allPieces[8][8])
{
    int m, n, total = 0;

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            
            if (allPieces[m][n].typeVal != EMPTY) {
                total += HASHBOARD[m][n];
                total += allPieces[m][n].typeVal;
                total += allPieces[m][n].colour;
                total += allPieces[m][n].mc;
            }
        }
    }

    return total;

}

/* initialises the hashboard with random numbers */
int 
initHashBoard()
{
    int m, n, r;

    srand((unsigned)time(NULL));

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            r = rand() / 100000;
            HASHBOARD[m][n] = r;
        }
    }

    return m * 8 + n;
}
