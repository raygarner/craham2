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

    return pos.colour == colour && pos.depth > depth && \
        compareAllPieces(transTable[index].board.allPieces, board.allPieces);
}

/* returns whether two all pieces are the same or not */
int
compareAllPieces(Piece a[8][8], Piece b[8][8])
{
    int m, n;

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            if (!comparePieces(a[m][n], b[m][n]))
                return 0;
        }
    }
    
    return 1;
}

/* returns whether two pieces are identical or not */
int comparePieces(Piece a, Piece b)
{
    return a.typeVal == b.typeVal && a.colour == b.colour && \
        a.mc == b.mc;
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
                total += HASHTYPE[typeValIndex(allPieces[m][n].typeVal)];
                total += HASHCOLOUR[allPieces[m][n].colour];
                total += allPieces[m][n].mc;
            }
        }
    }

    return total;

}

/* returns the index for a type val */
int
typeValIndex(int typeVal)
{
    switch(typeVal) {
    case PAWN :
        return 0;

    case KNIGHT :
        return 1;

    case BISHOP :
        return 2;

    case ROOK :
        return 3;

    case QUEEN :
        return 4;

    case KING :
        return 5;

    default :
        return -1;
    }

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

    for (m = 0; m <= 1; m++) {
        HASHCOLOUR[m] = rand() / 1000000;
    }

    for (m = 0; m <= 5; m++) {
        HASHTYPE[m] = rand() / 10000000;
    }

    return m * 8 + n;
}
