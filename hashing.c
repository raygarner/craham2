#include "interface.h"

/* generates an index to use based off of the board layout */
int
genHashCode(Piece allPieces[8][8])
{
    int m, n, total = 0;

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            
            if (allPieces[m][n].typeVal != EMPTY) {
                total += HASHBOARD[m][n];
            }
        }
    }

    return total;

}

/* initialises the hashboard with random numbers */
int 
initHashBoard()
{
    int m, n, r;

    srand((unsigned)time(NULL));

    for (m = 0; m < SIDE; m++) {
        for (n = 0; n < SIDE; n++) {
            r = rand();
            HASHBOARD[m][n] = r;
            printf("%d\n", r);

        }
    }

    return m * 8 + n;
}
