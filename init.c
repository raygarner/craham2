#include "interface.h"

/* adds pieces to a board */
Board 
addAllPieces(Board board)
{
    Piece piece;

    addPawns(1, BLACK, board.allPieces);
    addPawns(6, WHITE, board.allPieces);

    addTwoPieces(0, ROOK, board.allPieces);
    addTwoPieces(1, KNIGHT, board.allPieces);
    addTwoPieces(2, BISHOP, board.allPieces);
    addTwoPieces(3, QUEEN, board.allPieces);
    addTwoPieces(4, KING, board.allPieces);
    addTwoPieces(5, BISHOP, board.allPieces);
    addTwoPieces(6, KNIGHT, board.allPieces);
    addTwoPieces(7, ROOK, board.allPieces);

    addEmptySquares(board.allPieces);

    board.kingm[WHITE] = 7;
    board.kingn[WHITE] = 4;

    board.kingm[BLACK] = 0;
    board.kingn[BLACK] = 4;

    return board;
}

/* adds a row of pawns to a board */
int 
addPawns(int row, int colour, Piece allPieces[8][8])
{
    int n;

    for (n = 0; n < SIDE; n++) {
        allPieces[row][n].typeVal = PAWN;
        allPieces[row][n].colour = colour;
        allPieces[row][n].mc = 0;
    }

    return n;
}

/* adds a piece two both sides */
int 
addTwoPieces(int column, int typeVal, Piece allPieces[8][8])
{
    Piece piece;

    piece.mc = 0;
    piece.typeVal = typeVal;

    allPieces[0][column] = piece;
    allPieces[0][column].colour = BLACK;

    allPieces[7][column] = piece;
    allPieces[7][column].colour = WHITE;

    return 0;
}

/* adds the empty squares to the board */
int 
addEmptySquares(Piece allPieces[8][8])
{
    int m, n;

    for (m = 2; m <= 5; m++) {
        for (n = 0; n < SIDE; n++) {
            allPieces[m][n].typeVal = EMPTY;
            allPieces[m][n].mc = -1;
            allPieces[m][n].colour = -1;

        }
    }

    return m * n;
}

/* initialises the hashboard with random numbers */
int 
initHashBoard()
{
    int m, n;

    srand((unsigned)time(NULL));

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            HASHBOARD[m][n] = rand();
        }
    }

}
