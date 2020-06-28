#include "interface.h"

/* consts */
const Action LASTACTION = LAST_ACTION;
const Piece EMPTYSQUARE = EMPTY_SQUARE;

/* vars */
int HASHBOARD[8][8];
Position transTable[TABLE];
int transTableOffset = 0;
