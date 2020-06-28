#include "interface.h"

/* finds the strongest move for a colour in a position */
Action 
findMostEpicMove(Pair ab, int depth, int colour, Board board)
{
    Action action;
    Action legalMoves[MOVES];
    int index = 0;
    
    //printf("%d %d\n", ab.a, ab.b);
    index = addAllLegalMoves(colour,board,legalMoves);



    if (index == 0) {
        //printBoard(board.allPieces);
        if (colour) {
            action.eval = -1000000;
        } else {
            action.eval = 1000000;
        }
    

        return action;

    }

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
        action.eval = totalMaterial(board.allPieces);

        return action;
    }


    legalMoves[index] = LASTACTION;
    /* sortMoves(colour, legalMoves);*/

    addEvals(ab,depth,colour,board,legalMoves);

    action = strongestMoveFromList(colour, legalMoves);

    index = genHashCode(board.allPieces);
    addPosToTable(index, colour, depth, board, action);
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

        index = genHashCode(board.allPieces); 

        
        if (positionMatch(colour, depth, board, index)) {
            bestMove = transTable[index].action;
        } else {
            newBoard = executeMove(legalMoves[i].m, legalMoves[i].n, legalMoves[i].movem, \
                legalMoves[i].moven, board);
            bestMove = findMostEpicMove(ab, depth-1, !colour, newBoard);

        }
        
        explore = dontExplore(ab, colour, bestMove.eval);


        if (explore.a) {
            legalMoves[i].eval = explore.b;
            legalMoves[i+1] = LASTACTION;
            return legalMoves;
        } else {
            legalMoves[i].eval = bestMove.eval;
            newBoard = board;
            ab = updateAB(ab,colour,bestMove.eval);
            i++;
        }
        

        /*
        legalMoves[i].eval = eval;
        newBoard = board;
        ab = updateAB(ab,colour,eval);
        i++;
        */
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

        if (eval >= ab.b) {
            explore.a = 1;
        } else {
            explore.a = 0;
        }
    } else {
        explore.b = ab.a;

        if (eval <= ab.a) {
            explore.a = 1;
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
