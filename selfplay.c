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
    int colour = WHITE;

    ab.a = -2000000;
    ab.b = 2000000;

    board = addAllPieces(board);

    while (1) {
        index = addAllLegalMoves(WHITE, board, legalMoves);
        legalMoves[index] = LASTACTION;

        printBoard(board.allPieces);
        printf("material: %d\n", totalMaterial(board.allPieces));

        start = clock();
        bestMove = findMostEpicMove(ab, DEPTH, colour, board);
        end = clock();
        printf("%ds\n", (end - start) / CLOCKS_PER_SEC);
        printf("predicted eval: %d\n", bestMove.eval);
        board = executeMove(bestMove.m, bestMove.n, bestMove.movem, \
            bestMove.moven, board);

        colour = !colour;
    }

    return 0;
}
