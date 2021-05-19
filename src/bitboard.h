#ifndef BITBOARD_H_
#define BITBOARD_H_

#define bitscan(bb) (bb & -bb)                      //you know it
#define BitBoard uint64_t

BitBoard coordToBB(int row, int col);
int* BBToCoord(BitBoard bb);
void printBB(BitBoard bb); 

#endif
