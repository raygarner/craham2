#include "interface.h"

/* orders the moves by eval after one turn */
Action *
sortMoves(int colour, Action *legalMoves)
{
    Action tmp;
    int i = 0;
    int swapped = 1;

    if (colour) {
        while (swapped) {
            swapped = 0;
            i = 0;

            while (!isLastAction(legalMoves[i+1])) {
                if (legalMoves[i].eval <  legalMoves[i+1].eval) {
                    tmp = legalMoves[i];
                    legalMoves[i] = legalMoves[i+1];
                    legalMoves[i+1] = tmp;
                    swapped = 1;
                }

                i++;
            }
        }
    } else {
        while (swapped) {
            swapped = 0;
            i = 0;

            while (!isLastAction(legalMoves[i+1])) {
                if (legalMoves[i].eval >  legalMoves[i+1].eval) {
                    tmp = legalMoves[i];
                    legalMoves[i] = legalMoves[i+1];
                    legalMoves[i+1] = tmp;
                    swapped = 1;
                }

                i++;
            }
        }
    }

    /*
    i = 0;

    printf("sorted moves for %d:\n", colour);
    while (!isLastAction(legalMoves[i])) {
        printf("%d: %d\n", i, legalMoves[i].eval); 
        i++;
    }

    printf("\n");
    */

    return legalMoves;
}

/* returns the action with the best eval */
Action 
strongestMoveFromList(int colour, Action *legalMoves)
{
    int i = 1;
    Action currentBest;

    currentBest = legalMoves[0];

    if (colour) {
        while (!isLastAction(legalMoves[i])) {
            if (legalMoves[i].eval > currentBest.eval) {
                currentBest = legalMoves[i];
            }

            i++;
        }
    } else {
        while (!isLastAction(legalMoves[i])) {
            if (legalMoves[i].eval < currentBest.eval) {
                currentBest = legalMoves[i];
            }

            i++;
        }
    }

    return currentBest;
}

/* adds a legal move to the list of all legal moves */
int
addOneLegalMove(int index, int m, int n, int movem, int moven, \
Action *legalMoves, Board board)
{
    legalMoves[index].m = m;
    legalMoves[index].n = n;
    legalMoves[index].movem = movem;
    legalMoves[index].moven = moven;
    legalMoves[index].eval = totalVal(executeMove(m,n,movem,moven,board));

    return ++index;
}

/* adds all the legal caps for one side */
int 
addAllLegalCaps(int colour, Board board, Action *legalMoves)
{
    int m, n, index = 0, count = 0;
    for (m = 0; m < SIDE; m++ ) {
        for (n = 0; n < SIDE; n++) {
            if (board.allPieces[m][n].colour == colour) {
                index = addLegalCaps(m,n,board,legalMoves,index); 
                count++;
            }
            
            if (count >= 16)
                return index;
        }
    }

    return index;
}

/* adds all the legal moves for one side */
int 
addAllLegalMoves(int colour, Board board, Action *legalMoves)
{
    int m, n, index = 0, count = 0;
    for (m = 0; m < SIDE; m++ ) {
        for (n = 0; n < SIDE; n++) {
            if (board.allPieces[m][n].colour == colour) {
                index = addLegalMoves(m,n,board,legalMoves,index); 
                count++;
            }
            
            if (count >= 16)
                return index;
        }
    }

    return index;
}

/* adds all legal caps for one piece */
int 
addLegalCaps(int m, int n, Board board, Action *legalMoves, int index)
{
    switch(board.allPieces[m][n].typeVal) {
    case PAWN :
        return legalPawnCaps(m, n, board, legalMoves, index);        

    case KNIGHT :
        return legalKnightCaps(m, n, board, legalMoves, index);

    case BISHOP :
        return legalBishopCaps(m, n, board, legalMoves, index);

    case ROOK :
        return legalRookCaps(m, n, board, legalMoves, index);

    case QUEEN :
        return legalQueenCaps(m, n, board, legalMoves, index);

    case KING :
        return legalKingCaps(m, n, board, legalMoves, index);

    default :
        return index; 
    }
}

/* adds all legal moves for one piece */
int 
addLegalMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    switch(board.allPieces[m][n].typeVal) {
    case PAWN :
        return legalPawnMoves(m, n, board, legalMoves, index);        

    case KNIGHT :
        return legalKnightMoves(m, n, board, legalMoves, index);

    case BISHOP :
        return legalBishopMoves(m, n, board, legalMoves, index);

    case ROOK :
        return legalRookMoves(m, n, board, legalMoves, index);

    case QUEEN :
        return legalQueenMoves(m, n, board, legalMoves, index);

    case KING :
        return legalKingMoves(m, n, board, legalMoves, index);

    default :
        return index; 
    }
}

/* adds legal moves for one pawn */
int
legalPawnMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    int colour = board.allPieces[m][n].colour;
    int dir = DIRECTION(colour);
    
    if (board.allPieces[m+dir][n].typeVal == EMPTY && \
    !willBeInCheck(m, n, dir, 0, board)) {
        index = addOneLegalMove(index,m,n,dir,0,legalMoves,board);

        if (board.allPieces[m+dir*2][n].typeVal == EMPTY && \
        m == PAWNHOMEROW(board.allPieces[m][n].colour)) {
            index = addOneLegalMove(index,m,n,dir*2,0,legalMoves,board);
        }
    }
    
    index = legalPawnCaps(m,n,board,legalMoves,index);
        
    return index;
}

/* adds the legal captures a pawn can make */
int
legalPawnCaps(int m, int n, Board board, Action *legalMoves, int index)
{
    int i;
    int colour = board.allPieces[m][n].colour;
    int dir = DIRECTION(colour);
    Piece piece;

    for (i = -1; i <= 1; i += 2) {
        piece = board.allPieces[m+dir][n+i];

        if (isOnBoard(m+dir, n+i) && !willBeInCheck(m, n, dir, i, board)) {
            if (piece.typeVal != EMPTY && \
            board.allPieces[m][n].colour != piece.colour) {
                index = addOneLegalMove(index,m,n,dir,i,legalMoves,board);
            }
            
            if (piece.typeVal == EMPTY && \
            board.allPieces[m][n+i].typeVal == PAWN && \
            board.allPieces[m][n+i].colour != colour \
            && board.allPieces[m][n+i].mc == 2) {
                index = addOneLegalMove(index,m,n,dir,i,legalMoves,board);
            }
        }
    }

    return index;
}

/* adds legal moves for one knight */
int
legalKnightMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    if (isOnBoard(m+1, n+2) && !willBeInCheck(m,n,1,2,board))
        index = knightMoveLegality(m, n,1,2,board,legalMoves,index);

    if (isOnBoard(m-1, n+2) && !willBeInCheck(m,n,-1,2,board))
        index = knightMoveLegality(m, n,-1,2,board,legalMoves,index);

    if (isOnBoard(m+1, n-2) && !willBeInCheck(m,n,1,-2,board))
        index = knightMoveLegality(m, n,1,-2,board,legalMoves,index);

    if (isOnBoard(m-1, n-2) && !willBeInCheck(m,n,-1,-2,board))
        index = knightMoveLegality(m, n,-1,-2,board,legalMoves,index);

    if (isOnBoard(m+2, n+1) && !willBeInCheck(m,n,2,1,board))
        index = knightMoveLegality(m, n,2,1,board,legalMoves,index);

    if (isOnBoard(m-2, n+1) && !willBeInCheck(m,n,-2,1,board))
        index = knightMoveLegality(m, n,-2,1,board,legalMoves,index);

    if (isOnBoard(m+2, n-1) && !willBeInCheck(m,n,2,-1,board))
        index = knightMoveLegality(m, n,2,-1,board,legalMoves,index);

    if (isOnBoard(m-2, n-1) && !willBeInCheck(m,n,-2,-1,board))
        index = knightMoveLegality(m, n,-2,-1,board,legalMoves,index);

    return index;
}

/* checks and adds one knight move to legal moves */
int
knightMoveLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    if (isValidTarget(m, n, movem, moven, board.allPieces)) {
        index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
    }

    return index;
}

/* adds legal captures for one knight */
int
legalKnightCaps(int m, int n, Board board, Action *legalMoves, int index)
{
    if (isOnBoard(m+1, n+2) && !willBeInCheck(m,n,1,2,board))
        index = knightCapLegality(m, n,1,2,board,legalMoves,index);

    if (isOnBoard(m-1, n+2) && !willBeInCheck(m,n,-1,2,board))
        index = knightCapLegality(m, n,-1,2,board,legalMoves,index);

    if (isOnBoard(m+1, n-2) && !willBeInCheck(m,n,1,-2,board))
        index = knightCapLegality(m, n,1,-2,board,legalMoves,index);

    if (isOnBoard(m-1, n-2) && !willBeInCheck(m,n,-1,-2,board))
        index = knightCapLegality(m, n,-1,-2,board,legalMoves,index);

    if (isOnBoard(m+2, n+1) && !willBeInCheck(m,n,2,1,board))
        index = knightCapLegality(m, n,2,1,board,legalMoves,index);

    if (isOnBoard(m-2, n+1) && !willBeInCheck(m,n,-2,1,board))
        index = knightCapLegality(m, n,-2,1,board,legalMoves,index);

    if (isOnBoard(m+2, n-1) && !willBeInCheck(m,n,2,-1,board))
        index = knightCapLegality(m, n,2,-1,board,legalMoves,index);

    if (isOnBoard(m-2, n-1) && !willBeInCheck(m,n,-2,-1,board))
        index = knightCapLegality(m, n,-2,-1,board,legalMoves,index);

    return index;
}

/* checks and adds one knight capture to legal moves */
int
knightCapLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    if (board.allPieces[m][n].colour != \
    board.allPieces[m+movem][n+moven].colour && \
    board.allPieces[m][n].typeVal != EMPTY) {
        index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
    }

    return index;
}

/* adds legal moves for one bishop */
int
legalBishopMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    index = bishopLineLegality(m,n,1,1,board,legalMoves,index);
    index = bishopLineLegality(m,n,-1,1,board,legalMoves,index);
    index = bishopLineLegality(m,n,1,-1,board,legalMoves,index);
    index = bishopLineLegality(m,n,-1,-1,board,legalMoves,index);

    return index;
}

/* checks and adds moves for one line of direction for a bishop */
int
bishopLineLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    while (isOnBoard(m+movem, n+moven) && \
    board.allPieces[m+movem][n+moven].typeVal == EMPTY) {
        if (!willBeInCheck(m,n,movem,moven,board)) {
            index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
        }
        movem = furtherFromZero(movem);
        moven = furtherFromZero(moven);
    }

    if (isOnBoard(m+movem, n+moven) &&
    board.allPieces[m+movem][n+moven].colour != board.allPieces[m][n].colour) {
        if (!willBeInCheck(m,n,movem,moven,board)) {
            index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
        }
    }

    return index;
}

/* adds legal moves for one bishop */
int
legalBishopCaps(int m, int n, Board board, Action *legalMoves, int index)
{
    index = bishopCapLegality(m,n,1,1,board,legalMoves,index);
    index = bishopCapLegality(m,n,-1,1,board,legalMoves,index);
    index = bishopCapLegality(m,n,1,-1,board,legalMoves,index);
    index = bishopCapLegality(m,n,-1,-1,board,legalMoves,index);

    return index;
}

/* checks and adds a capture for one line of direction for a bishop */
int
bishopCapLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    while (isOnBoard(m+movem, n+moven) && \
    board.allPieces[m+movem][n+moven].typeVal == EMPTY) {
        movem = furtherFromZero(movem);
        moven = furtherFromZero(moven);
    }

    if (isOnBoard(m+movem, n+moven) &&
    board.allPieces[m+movem][n+moven].colour != board.allPieces[m][n].colour) {
        if (!willBeInCheck(m,n,movem,moven,board)) {
            index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
        }
    }

    return index;
}

/* adds the legal moves one rook can make */
int
legalRookMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    index = rookLineLegality(m,n,1,0,board,legalMoves,index);
    index = rookLineLegality(m,n,-1,0,board,legalMoves,index);
    index = rookLineLegality(m,n,0,1,board,legalMoves,index);
    index = rookLineLegality(m,n,0,-1,board,legalMoves,index);

    return index;
}

/* checks and adds moves for one line of direction for a rook */
int
rookLineLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    while (isOnBoard(m+movem, n+moven) && \
    board.allPieces[m+movem][n+moven].typeVal == EMPTY) {
        if (!willBeInCheck(m,n,movem,moven,board)) {
            index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
        }

        if (movem == 0) {
            moven = furtherFromZero(moven);
        } else {
            movem = furtherFromZero(movem);
        }
    }

    if (isOnBoard(m+movem, n+moven) &&
    board.allPieces[m+movem][n+moven].colour != board.allPieces[m][n].colour) {
        if (!willBeInCheck(m,n,movem,moven,board)) {
            index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
        }
    }

    return index;
}

/* adds the legal caps one rook can make */
int
legalRookCaps(int m, int n, Board board, Action *legalMoves, int index)
{
    index = rookCapLegality(m,n,1,0,board,legalMoves,index);
    index = rookCapLegality(m,n,-1,0,board,legalMoves,index);
    index = rookCapLegality(m,n,0,1,board,legalMoves,index);
    index = rookCapLegality(m,n,0,-1,board,legalMoves,index);

    return index;
}
/* checks and adds caps for one line of direction for a rook */
int
rookCapLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    while (isOnBoard(m+movem, n+moven) && \
    board.allPieces[m+movem][n+moven].typeVal == EMPTY) {
        if (movem == 0) {
            moven = furtherFromZero(moven);
        } else {
            movem = furtherFromZero(movem);
        }
    }

    if (isOnBoard(m+movem, n+moven) &&
    board.allPieces[m+movem][n+moven].colour != board.allPieces[m][n].colour) {
        if (!willBeInCheck(m,n,movem,moven,board)) {
            index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
        }
    }

    return index;
}

/* adds the legal moves one queen can make */
int
legalQueenMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    index = legalRookMoves(m, n, board, legalMoves, index);
    index = legalBishopMoves(m, n, board, legalMoves, index);

    return index;
}

/* adds the legal moves one queen can make */
int
legalQueenCaps(int m, int n, Board board, Action *legalMoves, int index)
{
    index = legalRookCaps(m, n, board, legalMoves, index);
    index = legalBishopCaps(m, n, board, legalMoves, index);

    return index;
}

/* adds the legal moves one king can make */
int
legalKingMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    int i;

    index = kingMoveLegality(m,n,1,1,board,legalMoves,index);
    index = kingMoveLegality(m,n,0,1,board,legalMoves,index);
    index = kingMoveLegality(m,n,1,0,board,legalMoves,index);
    index = kingMoveLegality(m,n,-1,-1,board,legalMoves,index);
    index = kingMoveLegality(m,n,1,-1,board,legalMoves,index);
    index = kingMoveLegality(m,n,-1,1,board,legalMoves,index);
    index = kingMoveLegality(m,n,0,-1,board,legalMoves,index);
    index = kingMoveLegality(m,n,-1,0,board,legalMoves,index);


    for (i = -2; i <= 2; i += 4) {
        if (validCastle(m,n,0,i,board)) {
            index = addOneLegalMove(index,m,n,0,i,legalMoves,board);
        }

    }

    return index;
}

/* checks and adds one move that the king can make */
int
kingMoveLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    if (isValidTarget(m,n,movem,moven,board.allPieces) && \
    !willBeInCheck(m,n,movem,moven,board) && \
    !kingsTooClose(m,n,movem,moven,board)) {
        index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
    }

    return index;
}

/* adds the legal moves one king can make */
int
legalKingCaps(int m, int n, Board board, Action *legalMoves, int index)
{
    index = kingCapLegality(m,n,1,1,board,legalMoves,index);
    index = kingCapLegality(m,n,0,1,board,legalMoves,index);
    index = kingCapLegality(m,n,1,0,board,legalMoves,index);
    index = kingCapLegality(m,n,-1,-1,board,legalMoves,index);
    index = kingCapLegality(m,n,1,-1,board,legalMoves,index);
    index = kingCapLegality(m,n,-1,1,board,legalMoves,index);
    index = kingCapLegality(m,n,0,-1,board,legalMoves,index);
    index = kingCapLegality(m,n,-1,0,board,legalMoves,index);

    return index;
}

/* checks and adds one move that the king can make */
int
kingCapLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    if (board.allPieces[m][n].colour != \
    board.allPieces[m+movem][n+moven].colour && \
    board.allPieces[m+movem][n+moven].typeVal != EMPTY && \
    isOnBoard(m+movem, n+moven) && \
    !willBeInCheck(m,n,movem,moven,board) && \
    !kingsTooClose(m,n,movem,moven,board)) {
        index = addOneLegalMove(index,m,n,movem,moven,legalMoves,board);
    }

    return index;
}
