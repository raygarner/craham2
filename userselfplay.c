#include "interface.h"

int main(int argc, char **argv)
{
    Board board;

    board = addAllPieces(board);

    while (1) {
        printBoard(board.allPieces);
        
        board = movePrompt(board);
    }
  
  return 0;
}
