#include "interface.h"

/* finds the strongest move for a colour in a position */
Action 
findMostEpicMove(Pair ab, int depth, int colour, Board board)
{
    Action action;
    Action legalMoves[MOVES];
    int index;
    

    if (depth == 0) {
        /*
        action.piece.typeVal = KING;
        action.piece.pos.m = 0;
        action.piece.pos.n = 0;
        action.piece.colour = colour;
        action.piece.mc = 0;
        action.move.m = 0;
        action.move.n = 0;
        */
        action.eval = totalVal(board);

        return action;
    }

    index = addAllLegalMoves(colour,board,legalMoves);



    if (index == 0) {
        if (colour) {
            action.eval = -1000000;
        } else {
            action.eval = 1000000;
        }
    

        return action;

    }

    legalMoves[index] = LASTACTION;
    addEvals(ab,depth,colour,board,legalMoves);


    return strongestMoveFromList(colour,legalMoves);

}

/* adds evals for moves */
Action *
addEvals(Pair ab, int depth, int colour, Board board, Action *legalMoves)
{
    Board newBoard; 
    int eval, i = 0;
    Pair explore;

    while (!isLastAction(legalMoves[i])) {
        newBoard = executeMove(legalMoves[i].m, legalMoves[i].n, legalMoves[i].movem, \
            legalMoves[i].moven, board);

        eval = findMostEpicMove(ab, depth-1, !colour, newBoard).eval;
        explore = dontExplore(ab, colour, eval);


        if (explore.a) {
            legalMoves[i].eval = explore.b;
            legalMoves[i+1] = LASTACTION;
            return legalMoves;
        } else {
            legalMoves[i].eval = eval;
            newBoard = board;
            ab = updateAB(ab,colour,eval);
            i++;
        }
    }

    return legalMoves;
}

/* returns whether a line should be explored and if not what eval to use */
Pair
dontExplore(Pair ab, int colour, int eval)
{
    Pair explore;

    if (colour) {
        explore.b = ab.b;

        if (eval >= ab.b)
            explore.a = 1;
        else
            explore.a = 0;
    } else {
        explore.b = ab.a;

        if (eval <= ab.a)
            explore.a = 1;
        else
            explore.a = 0;
    }

    return explore;
}

/* updates alpha and beta based on last eval */
Pair
updateAB(Pair ab, int colour, int eval)
{
    if (colour) {
        if (eval > ab.a) {
            ab.a= eval;
        }
    } else {
        if (eval < ab.b) {
            ab.b= eval;
        }
    }

    return ab;
}

/* returns the action with the best eval */
Action 
strongestMoveFromList(int colour, Action *legalMoves)
{
    int i = 1;
    Action currentBest;

    currentBest = legalMoves[0];

    /* written this way to reduce colour case checks */
    /* even through its ugly*/
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
    int dir = DIRECTION(board.allPieces[m][n].colour);
    int i;
    Piece piece;
    
    
    if (board.allPieces[m+dir][n].typeVal == EMPTY && \
    !willBeInCheck(m, n, dir, 0, board)) {
        legalMoves[index].m = m;
        legalMoves[index].n = n;
        legalMoves[index].movem = dir;
        legalMoves[index].moven = 0;
        index++;

        if (board.allPieces[m+dir*2][n].typeVal == EMPTY && \
        m == PAWNHOMEROW(board.allPieces[m][n].colour)) {
            legalMoves[index] = legalMoves[index-1];
            legalMoves[index].movem = dir * 2;
            index++;
        }
    }

    for (i = -1; i <= 1; i += 2) {
        piece = board.allPieces[m+dir][n+i];

        if (isOnBoard(m+dir, n+i) && !willBeInCheck(m, n, dir, i, board)) {
            if (piece.typeVal != EMPTY && \
            board.allPieces[m][n].colour != piece.colour) {
                legalMoves[index].m = m;
                legalMoves[index].n = n;
                legalMoves[index].movem = dir;
                legalMoves[index].moven = i;
                index++;
            }
            
            if (piece.typeVal == EMPTY && \
            board.allPieces[m][n+i].typeVal == PAWN && \
            board.allPieces[m][n+i].colour != board.allPieces[m][n].colour \
            && board.allPieces[m][n+i].mc == 2) {
                legalMoves[index].m = m;
                legalMoves[index].n = n;
                legalMoves[index].movem = dir;
                legalMoves[index].moven = i;
                index++;
            }
        }
    }
        
    return index;
}

/* adds legal moves for one knight */
int
legalKnightMoves(int m, int n, Board board, Action *legalMoves, int index)
{

    Board newBoard = board;

    newBoard.allPieces[m][n] = EMPTYSQUARE;

    if (inCheck(board.allPieces[m][n].colour, newBoard)) {
        return index;
    }

    index = knightMoveLegality(m, n,1,2,board,legalMoves,index);
    index = knightMoveLegality(m, n,-1,2,board,legalMoves,index);
    index = knightMoveLegality(m, n,1,-2,board,legalMoves,index);
    index = knightMoveLegality(m, n,-1,-2,board,legalMoves,index);
    index = knightMoveLegality(m, n,2,1,board,legalMoves,index);
    index = knightMoveLegality(m, n,-2,1,board,legalMoves,index);
    index = knightMoveLegality(m, n,2,-1,board,legalMoves,index);
    index = knightMoveLegality(m, n,-2,-1,board,legalMoves,index);

    return index;
}

/* checks and adds one knight move to legal moves */
int
knightMoveLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    if (isValidTarget(m, n, movem, moven, board.allPieces)) {
        legalMoves[index].m = m;
        legalMoves[index].n = n;
        legalMoves[index].movem = movem;
        legalMoves[index].moven = moven;
        index++;
    }

    return index;
}

/* adds legal moves for one bishop */
int
legalBishopMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    /*
    if (isOnBoard(m+1, n+1) && !willBeInCheck(m, n, 1, 1, board)) {
        index = bishopLineLegality(m,n,1,1,board,legalMoves,index);
        index = bishopLineLegality(m,n,-1,-1,board,legalMoves,index);
    }


    if (isOnBoard(m-1, n+1) && !willBeInCheck(m, n, -1, 1, board)) {
        index = bishopLineLegality(m,n,-1,1,board,legalMoves,index);
        index = bishopLineLegality(m,n,1,-1,board,legalMoves,index);
    }
    */

    if (isOnBoard(m+1, n+1) && !willBeInCheck(m,n,1,1,board))
        index = bishopLineLegality(m,n,1,1,board,legalMoves,index);

    if (isOnBoard(m-1, n+1) && !willBeInCheck(m,n,-1,1,board))
        index = bishopLineLegality(m,n,-1,1,board,legalMoves,index);

    if (isOnBoard(m+1, n-1) && !willBeInCheck(m,n,1,-1,board))
        index = bishopLineLegality(m,n,1,-1,board,legalMoves,index);
    
    if (isOnBoard(m-1, n-1) && !willBeInCheck(m,n,-1,-1,board))
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
        legalMoves[index].m = m;
        legalMoves[index].n = n;
        legalMoves[index].movem = movem;
        legalMoves[index].moven = moven;
        index++;
        movem = furtherFromZero(movem);
        moven = furtherFromZero(moven);
    }

    if (isOnBoard(m+movem, n+moven) &&
    board.allPieces[m+movem][n+moven].colour != board.allPieces[m][n].colour) {
        legalMoves[index].m = m;
        legalMoves[index].n = n;
        legalMoves[index].movem = movem;
        legalMoves[index].moven = moven;
        index++;
    }

    return index;
}

/* adds the legal moves one rook can make */
int
legalRookMoves(int m, int n, Board board, Action *legalMoves, int index)
{
    /*
    if (isOnBoard(m+1, n) && !willBeInCheck(m, n, 1, 0, board)) {
        index = rookLineLegality(m,n,1,0,board,legalMoves,index);
        index = rookLineLegality(m,n,-1,0,board,legalMoves,index);
    }
    
    if (isOnBoard(m, n+1) && !willBeInCheck(m, n, 0, 1, board)) {
        index = rookLineLegality(m,n,0,1,board,legalMoves,index);
        index = rookLineLegality(m,n,0,-1,board,legalMoves,index);
    }
    */

    if (isOnBoard(m+1, n) && !willBeInCheck(m,n,1,0,board))
        index = rookLineLegality(m,n,1,0,board,legalMoves,index);

    if (isOnBoard(m-1, n) && !willBeInCheck(m,n,-1,0,board))
        index = rookLineLegality(m,n,-1,0,board,legalMoves,index);

    if (isOnBoard(m, n-1) && !willBeInCheck(m,n,0,-1,board))
        index = rookLineLegality(m,n,-1,0,board,legalMoves,index);

    if (isOnBoard(m, n+1) && !willBeInCheck(m,n,0,1,board))
        index = rookLineLegality(m,n,-1,0,board,legalMoves,index);

    return index;
}

/* checks and adds moves for one line of direction for a rook */
int
rookLineLegality(int m, int n, int movem, int moven, Board board, \
Action *legalMoves, int index)
{
    
    while (isOnBoard(m+movem, n+moven) && \
    board.allPieces[m+movem][n+moven].typeVal == EMPTY) {
        legalMoves[index].m = m;
        legalMoves[index].n = n;
        legalMoves[index].movem = movem;
        legalMoves[index].moven = moven;
        index++;

        if (movem == 0) {
            moven = furtherFromZero(moven);
        } else {
            movem = furtherFromZero(movem);
        }
    }

    if (isOnBoard(m+movem, n+moven) &&
    board.allPieces[m+movem][n+moven].colour != board.allPieces[m][n].colour) {
        legalMoves[index].m = m;
        legalMoves[index].n = n;
        legalMoves[index].movem = movem;
        legalMoves[index].moven = moven;
        index++;
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
            legalMoves[index].m = m;
            legalMoves[index].n = n;
            legalMoves[index].movem = 0;
            legalMoves[index].moven = i;
            index++;
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
        legalMoves[index].m = m;
        legalMoves[index].n = n;
        legalMoves[index].movem = movem;
        legalMoves[index].moven = moven;
        index++;
    }

    return index;
}
