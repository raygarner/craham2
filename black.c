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
    /*
    initHashBoard();
    initTransTable();
    */
    
    /*
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
    */
    
    bestMove.m = 0;
    bestMove.n = 0;
    bestMove.movem = 0;
    bestMove.moven = 0;

    while (1) {

        start = clock();
        bestMove = findMostEpicMove(ab, DEPTH, WHITE, board);
        end = clock();
        printf("%ds\n", (end - start) / CLOCKS_PER_SEC);
        printf("predicted eval: %d\n", bestMove.eval);
        board = executeMove(bestMove.m, bestMove.n, bestMove.movem, \
            bestMove.moven, board);

        printBoard(board.allPieces);
        printf("craham's move: (%d,%d) (%d,%d)\n", bestMove.m, bestMove.n, \
            bestMove.movem, bestMove.moven);
        printf("material: %d\n", totalMaterial(board.allPieces));

        board = movePrompt(board);

        printBoard(board.allPieces);
    }

    return 0;
}
