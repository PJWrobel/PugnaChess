#ifndef BITBOARD_H_
#define BITBOARD_H_

#define bitscan(bb) (bb & -bb)                      //you know it
#define BitBoard uint64_t

BitBoard coordToBB(int row, int col);
int* BBToCoord(BitBoard bb);
_Bool isPopCnt1(BitBoard bb);
size_t popCount(BitBoard bb);
size_t popCountFast(BitBoard bb);
void printBB(BitBoard bb); 

#endif
