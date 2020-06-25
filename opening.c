#include "interface.h"

int main(int argc, char **argv)
{
    Board board;
    Action legalMoves[MOVES];
    int index;
    int i = 0;
    Action bestMove;
    Pair ab;
    int start, end;

    ab.a = -2000000;
    ab.b = 2000000;

    board = addAllPieces(board);

    
    while (1) {
        printBoard(board.allPieces);
        
        board = movePrompt(board);
        index = addAllLegalMoves(WHITE, board, legalMoves);
        legalMoves[index] = LASTACTION;

        while (!isLastAction(legalMoves[i])) {
            printf("%d %d\t %d %d\n", legalMoves[i].m, legalMoves[i].n, legalMoves[i].movem, legalMoves[i].moven);
            i++;
        }

        i = 0;
    }
    

    /*
    while (1) {
        printBoard(board.allPieces);
        printf("eval: %d\n", totalVal(board));

        board = movePrompt(board);

        printBoard(board.allPieces);

        start = clock();
        bestMove = findMostEpicMove(ab, 7, BLACK, board);
        end = clock();
        printf("%ds\n", (end - start) / CLOCKS_PER_SEC);
        printf("pred eval: %d\n", bestMove.eval);
        board = executeMove(bestMove.m, bestMove.n, bestMove.movem, \
            bestMove.moven, board);

    }
    */

    return 0;
}
