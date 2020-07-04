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

    bestMove.m = 0;
    bestMove.n = 0;
    bestMove.movem = 0;
    bestMove.moven = 0;

    while (1) {
        printBoard(board.allPieces);
        printf("craham's move: (%d,%d) (%d,%d)\n", bestMove.m, bestMove.n, \
            bestMove.movem, bestMove.moven);
        printf("eval: %d\n", totalVal(board));

        board = movePrompt(board);

        printBoard(board.allPieces);

        start = clock();
        bestMove = findMostEpicMove(ab, DEPTH, BLACK, board);
        end = clock();
        printf("%ds\n", (end - start) / CLOCKS_PER_SEC);
        printf("predicted eval: %d\n", bestMove.eval);
        board = executeMove(bestMove.m, bestMove.n, bestMove.movem, \
            bestMove.moven, board);

    }

    return 0;
}
