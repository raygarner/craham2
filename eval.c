#include "interface.h"

/* returns the eval for a board */
int 
totalVal(Board board)
{
    int material, pos;

    material = totalMaterial(board.allPieces);
    pos = evalAllPieces(board.allPieces);

    return material + pos;
}

/* returns the material eval for a position */
int 
totalMaterial(Piece allPieces[8][8])
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

/* calculates the index of the pos for use with a bitboard */
int 
calcIndex(int colour, int m, int n)
{
    if (colour == BLACK)
        m = 7 - m;

    return m * 8 + n;
}

/* returns the positional eval for one side */
int
evalAllPieces(Piece allPieces[8][8])
{
    int m, n, total = 0, count = 0;

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            if (allPieces[m][n].colour) {
                total += evalPiece(m, n, allPieces);
                count++;
            } else {
                total -= evalPiece(m, n, allPieces);
                count++;
            }

            if (count >= 32) {
                return total;
            }
        }
    }

    return total;

}

/* returns the positional eval for a piece */
int
evalPiece(int m, int n, Piece allPieces[8][8])
{
    switch(allPieces[m][n].typeVal) {
    case PAWN :
        return evalPawn(calcIndex(allPieces[m][n].colour, m, n));

    case BISHOP :
        return evalBishop(calcIndex(allPieces[m][n].colour, m, n));

    case KNIGHT :
        return evalKnight(calcIndex(allPieces[m][n].colour, m, n));

    case ROOK :
        return evalRook(calcIndex(allPieces[m][n].colour, m, n));

    case KING :
        return evalKing(calcIndex(allPieces[m][n].colour, m, n));

    default :
        return 0;
    }

}

/* returns the positional eval for a knight */
int
evalKnight(int index)
{
    static const int squares[] = {-50,-40,-30,-30,-30,-30,-40,-50,
                                -40,-20,  0,  0,  0,  0,-20,-40,
                                -30,  0, 10, 15, 15, 10,  0,-30,
                                -30,  5, 15, 10, 10, 15,  5,-30,
                                -30,  0, 15, 20, 20, 15,  0,-30,
                                -30,  5, 10, 15, 15, 10,  5,-30,
                                -40,-20,  0,  5,  5,  0,-20,-40,
                                -50,-40,-30,-30,-30,-30,-40,-50};

    return squares[index];
}

/* returns the positional eval for a bishop */
int
evalBishop(int index)
{
    static const int squares[] = {-20,-10,-10,-10,-10,-10,-10,-20,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -10,  0,  5, 10, 10,  5,  0,-10,
                                -10, 10,  5, 10, 10,  5, 10,-10,
                                -10,  0, 10, 10, 10, 10,  0,-10,
                                -10, 10, 10, -5, -5, 10, 10,-10,
                                -10, 10,  5,  5,  5,  5, 10,-10,
                                -20,-10,-10,-10,-10,-10,-10,-20};
    
    return squares[index];
}

/* returns the position eval for a rook */
int
evalRook(int index)
{
     static const int squares[] = { 0,  0,  0,  0,  0,  0,  0,  0,
                              5, 10, 10, 10, 10, 10, 10,  5,
                             -5,  0,  0,  0,  0,  0,  0, -5,
                             -5,  0,  0,  0,  0,  0,  0, -5,
                             -5,  0,  0,  0,  0,  0,  0, -5,
                             -5,  0,  0,  0,  0,  0,  0, -5,
                             -5,  0,  0,  0,  0,  0,  0, -5,
                              0,  0,  0,  5,  5,  0,  0,  0};
    return squares[index];
}

/* returns the position eval for a pawn */
int 
evalPawn(int index)
{
   static const int squares[] = { 0,  0,  0,  0,  0,  0,  0,  0,
                                50, 50, 50, 50, 50, 50, 50, 50,
                                10, 10, 20, 30, 30, 20, 10, 10,
                                 5,  5, 10, 20, 20, 10,  5,  5,
                                 0,  0,  0, 20, 20,-25,-20,  0,
                                 5, -5,-10,  0,  0,-10, -5,  5,
                                 5, 10, 10,-20,-20, 10, 10,  5,
                                 0,  0,  0,  0,  0,  0,  0,  0};

    return squares[index];
}

/* returns the position eval for a king */
int
evalKing(int index)
{
    static const int squares[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0,-20,-20,-20,0,0,
                                  0, 0, 0,-20,-20,-20,0,0};

    return squares[index];
}
