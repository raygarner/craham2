#include "interface.h"

/* init trans table */
int
initTransTable()
{
    int i;

    for (i = 0; i < TABLE; i++) {
        transTable[i].depth = -1;
    }
    
    return i;
}

/* returns whether the position in the table at index matches what is passed */
int
positionMatch(int colour, int depth, Board board, int index)
{
    Position pos = transTable[index];

    return pos.colour == colour && pos.depth > depth;
}

/* adds a position to the transtable */
int
addPosToTable(int index, int colour, int depth, Board board, Action action)
{
    transTable[index].board = board;
    transTable[index].depth = depth;
    transTable[index].colour = colour;
    transTable[index].action = action;

    return index;
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

    //srand((unsigned)time(NULL));

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            r = rand() / 100000;
            HASHBOARD[m][n] = r;
        }
    }

    return m * 8 + n;
}
