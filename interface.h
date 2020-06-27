#ifndef _INTERFACE
#define _INTERFACE

/* libs */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* defs */
#define SIDE 8
#define MOVES 256
#define CAPTURED -1
#define DEPTH 6

#define LAST_ACTION { .m = 0 , .n = 0, .movem = 0, .moven = 0, \
    .eval = 0 }
#define EMPTY_SQUARE { .typeVal = 0, .colour = -1, .mc = -0 }

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

#define HOMEROW(A) ((A) ? (7) : (0))
#define PAWNHOMEROW(A) ((A) ? (6) : (1))
#define DIRECTION(A) ((A) ? (-1) : (1))

enum colour {
    BLACK,
    WHITE
};

enum pieceType {
    KING = 1000,
    QUEEN = 900,
    ROOK = 500,
    BISHOP = 350,
    KNIGHT = 300,
    PAWN = 100,
    EMPTY = 0
};


/* types */
typedef struct Pair {
    int a;
    int b;
} Pair;

typedef struct Piece {
    int typeVal;
    int colour;
    int mc;
} Piece;

typedef struct Action {
    int m;
    int n;
    int movem;
    int moven;
    int eval;
} Action;

typedef struct Board {
    Piece allPieces[SIDE][SIDE];
    int kingm[2];
    int kingn[2];
} Board;

/* init.c */
Board addAllPieces(Board board);
int addPawns(int row, int colour, Piece allPieces[8][8]);
int addTwoPieces(int column, int typeVal, Piece allPieces[8][8]);
int addEmptySquares(Piece allPieces[8][8]);

/* ui.c */
int printBoard(Piece allPieces[8][8]);
char getPieceIcon(int typeVal, int colour);
Board movePrompt(Board board);

/* util.c */
extern const Action LASTACTION;
extern const Piece EMPTYSQUARE;
int furtherFromZero(int num);
int closerToZero(int num);
int isInBounds(int x);
int isOnBoard(int m, int n);
int isValidTarget(int m, int n, int movem, int moven, Piece allPieces[8][8]);
int diagMovePathEmpty(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int straightPathEmpty(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int isLShaped(int m, int n);
int validKnightMove(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int validBishopMove(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int validRookMove(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int validKingMove(int m, int n, int movem, int moven, Board board);
int kingsTooClose(int m, int n, int movem, int moven, Board board);
int validCastle(int m, int n, int movem, int moven, Board board);
int isBasicPawnMove(int colour, int movem, int moven);
int isPawnForwardTwo(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int isPawnCapture(int colour, int movem, int moven);
int validPawnMove(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int validEnPassant(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int validMove(int m, int n, int movem, int moven, Board board);
int isThreatened(int m, int n, Board board, int colour);
int inCheck(int colour, Board board);
Board executeMove(int m, int n, int movem, int moven, Board board);
int resetEnemyPawns(int colour, Piece allPieces[8][8]);
Board executeCastle(int m, int moven, Board board);
int captureEnPassant(int m, int n, int movem, int moven, \
    Piece allPieces[8][8]);
int copyAllpieces(Piece a[8][8], Piece b[8][8]);
int willBeInCheck(int m, int n, int movem, int moven, Board board);
int isLastAction(Action action);

/* eval.c */
int totalVal(Board board);
int totalMaterial(Piece allPieces[8][8]);

/* search.c */
Action findMostEpicMove(Pair ab, int depth, int colour, Board board);
Action *addEvals(Pair ab, int depth, int colour, Board board, \
    Action *legalMoves);
Pair dontExplore(Pair ab, int colour, int eval);
Pair updateAB(Pair ab, int colour, int eval);
Action strongestMoveFromList(int colour, Action *legalMoves);
int addAllLegalMoves(int colour, Board board, Action *legalMoves);
int addLegalMoves(int m, int n, Board board, Action *legalMoves, int index);
int legalPawnMoves(int m, int n, Board board, Action *legalMoves, int index);
int legalKnightMoves(int m, int n, Board board, Action *legalMoves, int index);
int knightMoveLegality(int m, int n, int movem, int moven, Board board, \
    Action *legalMoves, int index);
int legalBishopMoves(int m, int n, Board board, Action *legalMoves, int index);
int bishopLineLegality(int m, int n, int movem, int moven, Board board, \
    Action *legalMoves, int index);
int legalRookMoves(int m, int n, Board board, Action *legalMoves, int index);
int rookLineLegality(int m, int n, int movem, int moven, Board board, \
    Action *legalMoves, int index);
int legalQueenMoves(int m, int n, Board board, Action *legalMoves, int index);
int legalKingMoves(int m, int n, Board board, Action *legalMoves, int index);
int kingMoveLegality(int m, int n, int movem, int moven, Board board, \
    Action *legalMoves, int index);

#endif
