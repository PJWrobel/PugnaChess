#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "bitboard.h"

#define bitscan(bb) (bb & -bb)
#define BitBoard uint64_t


BitBoard coordToBB(int row, int col)
{   BitBoard bb = 1;
    bb <<= row*8 + col;
    return bb;
}      //bitboard for 8x8 coords

int* BBToCoord(BitBoard bb)
{   if(!bb)
        return NULL;
    int rank, file;
    BitBoard mask = 0x101010101010101;             //1's on column a
    for(file = 0; ! (mask & bb); file++, bb >>= 1);    //shift board left until piece in a column
    for(rank = 0; bb; rank++, bb >>= 8);
    int *coords = malloc(8); //TODO: free?
    coords[0] = file;
    coords[1] = rank-1;
    return coords;
}
_Bool isPopCnt1(BitBoard bb){ return bb & (bb-1); }

size_t popCount (BitBoard bb)   //chessprogramming.org/Population_Count
{   size_t count = 0;
    while (bb)
    {   count++;
        bb &= bb - 1; // reset LS1B
    }
    return count;
}
const BitBoard k1 = 0x5555555555555555; /*  -1/3   */
const BitBoard k2 = 0x3333333333333333; /*  -1/5   */
const BitBoard k4 = 0x0f0f0f0f0f0f0f0f; /*  -1/17  */
const BitBoard kf = 0x0101010101010101; /*  -1/255 */       //also from cpw

size_t popCountFast(BitBoard bb)
{   bb =  bb       - ((bb >> 1)  & k1);
    bb = (bb & k2) + ((bb >> 2)  & k2);
    bb = (bb       +  (bb >> 4)) & k4 ; 
    bb = (bb * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
    return (size_t) bb;
}

void printBB(BitBoard bb)
{   for(int i=7; i>=0; i--)          //top left to bottom right
    {   for(int j=0; j<8;j++)
            printf("%c ", bb & coordToBB(i,j) ? '1' : '.');
        puts("");
    }
    puts("");
}

