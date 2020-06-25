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
        index = addAllLegalMoves(BLACK, board, legalMoves);
        legalMoves[index] = LASTACTION;


        printf("LEGAL MOVES:\n");
        while (!isLastAction(legalMoves[i])) {
            printf("%d %d\t %d %d\n", legalMoves[i].m, legalMoves[i].n, legalMoves[i].movem, legalMoves[i].moven);
            i++;
        }

        i = 0;
    }
  
  return 0;
}
