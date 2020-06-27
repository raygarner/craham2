#include "interface.h"


/* returns an int one further from zero than the int passed */
int
furtherFromZero(int num)
{
    if (num < 0) {
        return num - 1;
    } else if (num > 0) {
        return num + 1;
    } else {
        return num;
    }
}

/* returns an int one closer to zero than the int passed */
int
closerToZero(int num)
{
    if (num < 0) {
        return num + 1;
    } else if (num > 0) {
        return num - 1;
    } else {
        return num;
    }
}

/* returns whether an integer is whithin the bounds of a chess board */
int
isInBounds(int x)
{
    return x >= 0 && x <= 7;
}

/* returns whether a position is on a chess board */
int
isOnBoard(int m, int n)
{
    return (isInBounds(m) && isInBounds(n));
}

/* returns whether a move obides by the universal chess rules */
int
isValidTarget(int m, int n, int movem, int moven, Piece allPieces[8][8])
{
    int targetm = m + movem;
    int targetn = n + moven;
    Piece targetP = allPieces[targetm][targetn];

    return isOnBoard(targetm, targetn) && \
    (targetP.typeVal == EMPTY || targetP.colour != allPieces[m][n].colour);
}

/* returns whether a diagonal move path is clear or not */
int
diagPathEmpty(int m, int n, int movem, int moven, Piece allPieces[8][8])
{
    movem = closerToZero(movem);
    moven = closerToZero(moven);

    while (movem != 0) {
        if (allPieces[m+movem][n+moven].typeVal != EMPTY)
            return 0;

        movem = closerToZero(movem);
        moven = closerToZero(moven);
    }

    return 1;
}

/* returns whether a straight move path is clear or not */
int
straightPathEmpty(int m, int n, int movem, int moven, \
Piece allPieces[8][8])
{
    movem = closerToZero(movem);
    moven = closerToZero(moven);

    while (movem != moven) {
        if (allPieces[m+movem][n+moven].typeVal != EMPTY)
            return 0;

        if (movem == 0) {
            moven = closerToZero(moven);
        } else {
            movem = closerToZero(movem);
        }
    }

    return 1;
}

/* returns whether a move is L shaped or not */
int
isLShaped(int m, int n)
{
    int am = abs(m);
    int an = abs(n);

    return (am == 2 && an == 1) || (am == 1 && an == 2);
}

/* returns whether a knight move is valid or not*/
int
validKnightMove(int m, int n, int movem, int moven, Piece allPieces[8][8])
{
    return isLShaped(movem, moven) && isValidTarget(m, n, movem, moven, allPieces);
}

/* returns whether a bishop move is valid or not*/
int
validBishopMove(int m, int n, int movem, int moven, Piece allPieces[8][8])
{
    return abs(movem) == abs(moven) && isValidTarget(m, n, movem, moven, allPieces)\
    && diagPathEmpty(m, n, movem, moven, allPieces);
}

/* returns whether a rook move is valid or not */
int
validRookMove(int m, int n, int movem, int moven, Piece allPieces[8][8])
{
    return (movem == 0 || moven == 0) && isValidTarget(m, n, movem, moven, allPieces)\
    && straightPathEmpty(m, n, movem, moven, allPieces);
}

/* returns whether a king move is valid or not
 * not including castling */
int
validKingMove(int m, int n, int movem, int moven, Board board)
{
    if (abs(movem) > 1 || abs(moven) > 1)
        return 0;

    if (!isValidTarget(m, n, movem, moven, board.allPieces))
        return 0;

    return !kingsTooClose(m,n,movem,moven,board);
}

/* returns whether the opposing kings are too close to each other */
int
kingsTooClose(int m, int n, int movem, int moven, Board board)
{
    int colour, diffm, diffn, enemyn, enemym;

    colour = !board.allPieces[m][n].colour;
    enemym = board.kingm[colour];
    enemyn = board.kingn[colour];

    diffm = (m+movem) - enemym;
    diffn = (n+moven) - enemyn;

    return abs(diffm) <= 1 && abs(diffn) <= 1;
}

/* returns whether a move is a valid castle */
int
validCastle(int m, int n, int movem, int moven, Board board)
{
    int side, start, end, i;
    

    if (!(board.allPieces[m][n].mc == 0 && movem == 0))
        return 0;


    if (moven == 2) {
        side = 1;
    } else if (moven == -2) {
        side = 0;
    } else {
        return 0;
    }

    if (side) {
        if (board.allPieces[m][7].mc != 0 || \
        board.allPieces[m][7].typeVal != ROOK)
            return 0;

        start = 4;
        end = 6;
    } else {
        if (board.allPieces[m][0].mc != 0 || \
        board.allPieces[m][0].typeVal != ROOK) {
            return 0;
        }

        start = 2;
        end = 4;
    }


    for (i = start; i <= end; i++) {
        if (board.allPieces[m][i].typeVal != KING \
        && board.allPieces[m][i].typeVal != EMPTY) {
            return 0;
        }

        if (isThreatened(m, i, board, board.allPieces[m][n].colour)) {
            return 0;
        }
    }

    
    return 1;
}

/* returns whether a move is the most basic kind of pawn move */
int
isBasicPawnMove(int colour, int movem, int moven)
{
    return moven == 0 && movem == DIRECTION(colour);
}

/* returns whether a move is a pawn forward two move
 * assumes target is empty */
int
isPawnForwardTwo(int m, int n, int movem, int moven, Piece allPieces[8][8])
{
    return allPieces[m][n].mc == 0 && moven == 0 && \
        movem == 2 * DIRECTION(allPieces[m][n].colour) && \
        allPieces[m+DIRECTION(allPieces[m][n].colour)][n].typeVal == EMPTY;

}

/* returns whether the movement of a pawn is a capture */
int
isPawnCapture(int colour, int movem, int moven)
{
    return movem == DIRECTION(colour) && abs(moven) == 1;
}

/* returns whether a pawn move is valid
 * not including en passant */
int
validPawnMove(int m, int n, int movem, int moven, Piece allPieces[8][8])
{

    int targetm, targetn;

    targetm = m+movem;
    targetn = n+moven;

    if (allPieces[targetm][targetn].colour == allPieces[m][n].colour)
        return 0;

    if (!isOnBoard(targetm, targetn))
        return 0;

    if (allPieces[targetm][targetn].typeVal == EMPTY) {
        if (isBasicPawnMove(allPieces[m][n].colour, movem, moven)) {
            return 1;
        } else if (isPawnForwardTwo(m, n, movem, moven, allPieces)) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (allPieces[m][n].colour != allPieces[targetm][targetn].colour && \
        isPawnCapture(allPieces[m][n].colour, movem, moven)) {
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

/* returns whether a move is a valid en passant */
int
validEnPassant(int m, int n, int movem, int moven, Piece allPieces[8][8])
{
    if (!(isValidTarget(m, n, movem, moven, allPieces) && \
    isPawnCapture(allPieces[m][n].colour, movem, moven)))
        return 0;

    return allPieces[m][n+moven].typeVal == PAWN && \
    allPieces[m][n+moven].mc == 2;
}

/* returns whether a move is valid or not */
int
validMove(int m, int n, int movem, int moven, Board board)
{
    switch(board.allPieces[m][n].typeVal) {
    case PAWN :
        return validPawnMove(m, n, movem, moven, board.allPieces) || \
            validEnPassant(m, n, movem, moven, board.allPieces);

    case KNIGHT :
        return validKnightMove(m, n, movem, moven, board.allPieces);

    case BISHOP :
        return validBishopMove(m, n, movem, moven, board.allPieces);

    case ROOK :
        return validRookMove(m, n, movem, moven, board.allPieces);

    case QUEEN :
        return validRookMove(m, n, movem, moven, board.allPieces) || \
            validBishopMove(m, n, movem, moven, board.allPieces);

    case KING :
        return validKingMove(m, n, movem, moven, board) || \
            validCastle(m, n, movem, moven, board);

    default :
        return 0;
    }
}

/* returns whether a piece is threatened 
 * only used to check for check and castle validation */
int
isThreatened(int m, int n, Board board, int colour)
{
    int searchm, searchn;
    int movem, moven;
//    int colour = board.allPieces[m][n].colour;
    int count = 0;

    for (searchm = 0; searchm < SIDE; searchm++) {
        for (searchn = 0; searchn < SIDE; searchn++) {
            
            if (count >= 16)
                return 0;

            if (board.allPieces[searchm][searchn].typeVal != EMPTY && \
            board.allPieces[searchm][searchn].colour != colour
            && board.allPieces[searchm][searchn].typeVal != KING) {
                count++;
                movem = m - searchm;
                moven = n - searchn;


                if (validMove(searchm, searchn, movem, moven, board)) {
                    return 1;
                }
            }
        }
    }


    return 0;
}

/* returns whether a side is in check or not */
int
inCheck(int colour, Board board)
{
    return isThreatened(board.kingm[colour],board.kingn[colour],board,colour);
}

/* returns whether a side will be in check after a move is made */
int
willBeInCheck(int m, int n, int movem, int moven, Board board)
{
    Board newBoard = executeMove(m, n, movem, moven, board);

    /*
    if (isOnBoard(m+movem, n+moven)) {
        board = executeMove(m, n, movem, moven, board);
    } else {
        return 0;
    }
    */


    return inCheck(board.allPieces[m][n].colour, newBoard);
}

