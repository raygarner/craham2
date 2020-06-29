#include "interface.h"

/* finds the strongest move for a colour in a position */
Action 
findMostEpicQuietMove(Pair ab, int depth, int colour, Board board)
{
    Action action;
    Action legalMoves[MOVES];
    int index, hashcode;
    
    index = addAllLegalCaps(colour,board,legalMoves);

    if (index == 0) {
        action.eval = totalMaterial(board.allPieces);

        return action;
    }

    legalMoves[index] = LASTACTION;

    addEvalsQuiet(ab,depth,colour,board,legalMoves);

    action = strongestMoveFromList(colour, legalMoves);

    return action;
}

/* adds evals for moves */
Action *
addEvalsQuiet(Pair ab, int depth, int colour, Board board, Action *legalMoves)
{
    Board newBoard; 
    int index, i = 0;
    Pair explore;
    Action bestMove;
    int eval = totalMaterial(board.allPieces);
    
    explore = dontExplore(ab, colour, eval);
    
    if (explore.a) {
        legalMoves[0].eval = explore.b;
        legalMoves[1] = LASTACTION;
        return legalMoves;
    }

    ab = updateAB(ab,colour,eval);

    while (!isLastAction(legalMoves[i])) {
        newBoard = executeCap(legalMoves[i].m, legalMoves[i].n, legalMoves[i].movem, \
            legalMoves[i].moven, board);

        bestMove = findMostEpicQuietMove(ab, depth-1, !colour, newBoard);
        
        explore = dontExplore(ab, colour, bestMove.eval);

        if (explore.a) {
            legalMoves[i].eval = explore.b;
            legalMoves[i+1] = LASTACTION;
            return legalMoves;
        } else {
            legalMoves[i].eval = bestMove.eval;
            ab = updateAB(ab,colour,bestMove.eval);
            i++;
        }
    }

    return legalMoves;
}

/* finds the strongest move for a colour in a position */
Action 
findMostEpicMove(Pair ab, int depth, int colour, Board board)
{
    Action action;
    Action legalMoves[MOVES];
    int index, hashcode;

    index = addAllLegalMoves(colour,board,legalMoves);

    if (index == 0) {
        if (colour) {
            action.eval = -1000000;
        } else {
            action.eval = 1000000;
        }

        return action;
    }

    if (depth == 0) {
        action = findMostEpicQuietMove(ab,depth,colour,board);

        return action;
        
    }

    legalMoves[index] = LASTACTION;

    addEvals(ab,depth,colour,board,legalMoves);

    action = strongestMoveFromList(colour, legalMoves);

    return action;
}

/* adds evals for moves */
Action *
addEvals(Pair ab, int depth, int colour, Board board, Action *legalMoves)
{
    Board newBoard; 
    int index, i = 0;
    Pair explore;
    Action bestMove;

    while (!isLastAction(legalMoves[i])) {
        newBoard = executeMove(legalMoves[i].m, legalMoves[i].n, legalMoves[i].movem, \
            legalMoves[i].moven, board);

        bestMove = findMostEpicMove(ab, depth-1, !colour, newBoard);
        
        explore = dontExplore(ab, colour, bestMove.eval);

        if (explore.a) {
            legalMoves[i].eval = explore.b;
            legalMoves[i+1] = LASTACTION;
            return legalMoves;
        } else {
            legalMoves[i].eval = bestMove.eval;
            ab = updateAB(ab,colour,bestMove.eval);
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

        if (eval >= ab.b) {
            explore.a = 1;
            explore.b = ab.b;
        } else {
            explore.a = 0;
        }
    } else {

        if (eval <= ab.a) {
            explore.a = 1;
            explore.b = ab.a;
        } else {
            explore.a = 0;
        }
    }

    return explore;
}

/* updates alpha and beta based on last eval */
Pair
updateAB(Pair ab, int colour, int eval)
{
    if (colour) {
        if (eval > ab.a) {
            ab.a = eval;
        }
    } else {
        if (eval < ab.b) {
            ab.b = eval;
        }
    }

    return ab;
}

/* checks if an action is the last action marker */
int
isLastAction(Action action)
{
    return action.m == LASTACTION.m && action.n == LASTACTION.n && \
    action.movem == LASTACTION.movem && action.moven == LASTACTION.moven && \
    action.eval == LASTACTION.eval;
}
