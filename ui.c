#include "interface.h"

/* prints the board */
int printBoard(Piece allPieces[8][8])
{
    Piece piece;
    int m, n, i;

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
            printf("\033[0m"); /* reset colour */
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
        if (!colour) {
            printf("\033[0;34m"); /* set colour to blue */
        }
        return 'k';

    case QUEEN :
        if (!colour) {
            printf("\033[0;34m"); /* set colour to blue */
        }
        return 'q';

    case ROOK :
        if (!colour) {
            printf("\033[0;34m"); /* set colour to blue */
        }
        return 'r';

    case BISHOP :
        if (!colour) {
            printf("\033[0;34m"); /* set colour to blue */
        }
        return 'b';

    case KNIGHT :
        if (!colour) {
            printf("\033[0;34m"); /* set colour to blue */
        }
        return 'n';

    case PAWN :
        if (!colour) {
            printf("\033[0;34m"); /* set colour to blue */
        }
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
    int m = -1, n = -1, movem = 9, moven = 9;

    while (1) {
        printf("Enter the move you wish to make: ");
        scanf("%d %d %d %d", &m, &n, &movem, &moven);

        if (!isOnBoard(abs(movem), abs(moven))) {
            printf("You cannot move a piece off of the board\n");
        } else if (!isOnBoard(m, n)) {
            printf("You cannot move from a position that is not on the board\n");
        } else if (willBeInCheck(m, n, movem, moven, board)) {
            printf("That move will leave you in check\n");
        } else if (board.allPieces[m][n].typeVal == EMPTY) {
            printf("You cannot move from a square that is empty\n");
        } else if (!validMove(m, n, movem, moven, board)) {
            printf("That piece cannot be moved like that\n");
        } else {
            break; 
        }
    }

    return executeMove(m, n, movem, moven, board);
}
