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

/* executes a move on the board and returns the new board
 * assumes move is legal */
Board
executeMove(int m, int n, int movem, int moven, Board board)
{
    int colour = board.allPieces[m][n].colour;

    if (board.allPieces[m][n].typeVal == KING) {
        board.kingm[colour] = m + movem;
        board.kingn[colour] = n + moven;
    }

    if (board.allPieces[m][n].typeVal == PAWN) {
        if (movem == 2 * DIRECTION(colour)) {
            board.allPieces[m][n].mc += 2;
        } else {
            board.allPieces[m][n].mc++;
        }

        if (m + movem == HOMEROW(!colour)) {
            board.allPieces[m][n].typeVal = QUEEN;
        }
    } else {
        board.allPieces[m][n].mc++;
    }
    
    if (board.allPieces[m][n].typeVal == KING && abs(moven) > 1) {
        board = executeCastle(m, moven, board);

    } else if (board.allPieces[m][n].typeVal == PAWN && moven != 0 && \
    board.allPieces[m+movem][n+moven].typeVal == EMPTY) {

        captureEnPassant(m, n, movem, moven, board.allPieces);

    } else {
        board.allPieces[m+movem][n+moven] = board.allPieces[m][n];

        board.allPieces[m][n] = EMPTYSQUARE;

    }
        
    resetEnemyPawns(colour, board.allPieces);

    return board;
}

/* executes a cap on the board and returns the new board
 * assumes move is legal */
Board
executeCap(int m, int n, int movem, int moven, Board board)
{
    int colour = board.allPieces[m][n].colour;

    if (board.allPieces[m][n].typeVal == KING) {
        board.kingm[colour] = m + movem;
        board.kingn[colour] = n + moven;
    }

    if (board.allPieces[m][n].typeVal == PAWN && \
    m + movem == HOMEROW(!colour)) {
            board.allPieces[m][n].typeVal = QUEEN;
    }
    
    board.allPieces[m][n].mc++;

    if (board.allPieces[m][n].typeVal == PAWN && moven != 0 && \
    board.allPieces[m+movem][n+moven].typeVal == EMPTY) {

        captureEnPassant(m, n, movem, moven, board.allPieces);

    } else {
        board.allPieces[m+movem][n+moven] = board.allPieces[m][n];

        board.allPieces[m][n] = EMPTYSQUARE;

    }
        
    resetEnemyPawns(colour, board.allPieces);

    return board;
}
/* resets the move count for the enemy pawns */
int
resetEnemyPawns(int colour, Piece allPieces[8][8])
{
    int m, n, count = 0;

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            if (allPieces[m][n].colour != colour && \
            allPieces[m][n].typeVal == PAWN) {
                allPieces[m][n].mc = 0;
                count++;
            }

            if (count >= 8)
                return count;

        }
    }

    return count;
}

/* executes a castle on the board */
Board
executeCastle(int m, int moven, Board board)
{
    int rookn, rookmoven, colour;

    colour = board.allPieces[m][7].colour;
    board.kingn[colour] = 4 + moven;

    board.allPieces[m][4].mc++;

    board.allPieces[m][4+moven] = board.allPieces[m][4];

    board.allPieces[m][4] = EMPTYSQUARE;

    if (moven == 2) {
        rookn = 7;
        rookmoven = -2;
    } else {
        rookn = 0;
        rookmoven = 3;
    }

    board.allPieces[m][rookn].mc++;

    board.allPieces[m][rookn+rookmoven] = board.allPieces[m][rookn];

    board.allPieces[m][rookn].typeVal = EMPTY;
    board.allPieces[m][rookn].mc = -1;
    board.allPieces[m][rookn].colour = -1;

    return board;
}

/* executes an en passant capture on the board */
int
captureEnPassant(int m, int n, int movem, int moven, Piece allPieces[8][8])
{
    allPieces[m][n].mc++;

    allPieces[m+movem][n+moven] = allPieces[m][n];

    allPieces[m][n] = EMPTYSQUARE;

    allPieces[m][n+moven] = allPieces[m][n];

    return 0;
}

