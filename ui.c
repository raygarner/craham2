#include "interface.h"

/* prints the board */
int printBoard(Piece allPieces[8][8])
{
    Piece piece;
    int m, n;

    printf("  ");
    for (m = 0; m < 8; m++) {
        printf("%d ", m);
    }
    printf("\n");

    for (m = 0; m < SIDE; m++) {
        printf("%d ", m);
        for (n = 0; n < SIDE; n++) {
            piece = allPieces[m][n];
            printf("%c", getPieceIcon(piece.typeVal, piece.colour));
            printf(" ");
        }
        printf("\n");
    }

    return m * n;
}

/* returns the icon to be printed for a piece type */
char getPieceIcon(int typeVal, int colour)
{
    switch(typeVal) {
    case KING :
        if (colour)
            return 'K';
        else
            return 'k';

    case QUEEN :
        if (colour)
            return 'Q';
        else
            return 'q';

    case ROOK :
        if (colour)
            return 'R';
        else
            return 'r';

    case BISHOP :
        if (colour)
            return 'B';
        else
            return 'b';

    case KNIGHT :
        if (colour)
            return 'N';
        else
            return 'n';

    case PAWN :
        if (colour)
            return 'P';
        else
            return 'p';

    case EMPTY :
        return '-';

    default :
        return '?';
    }

    return '?';
}

/* prompts the user for a move and makes it if its valid */
Board movePrompt(Board board)
{
    int m = -1, n = -1, movem = 0, moven = 0;

    /*
    printf("Enter the move you wish to make: ");
    scanf("%d %d %d %d", &m, &n, &movem, &moven);

    if (board.allPieces[m][n].typeVal == EMPTY) {
        printf("you're trying to move an empty square\n");
        return board;
    }


    if (!validMove(m, n, movem, moven, board)) {
        printf("INVALID MOVE\n");
    } else if (willBeInCheck(m, n, movem, moven, board)) {
        printf("YOU CANNOT MOVE INTO CHECK\n");
    } else {
        board = executeMove(m, n, movem, moven, board);
    }
    */

    while (!isOnBoard(abs(movem), abs(moven)) || \
    !validMove(m, n, movem, moven, board) || \
    willBeInCheck(m, n, movem, moven, board) || \
    board.allPieces[m][n].typeVal == EMPTY) {
        printf("Enter the move you wish to make: ");
        scanf("%d %d %d %d", &m, &n, &movem, &moven);
    }

    return executeMove(m, n, movem, moven, board);

    //return board;
}
