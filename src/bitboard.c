#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "bitboard.h"

#define bitscan(bb) (bb & -bb)                      //you know it
#define BitBoard uint64_t


BitBoard coordToBB(int row, int col)
{   BitBoard bb = 1;
    bb <<= row*8 + col;
    return bb;
}      //bitboard mask for 8x8 coords

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


void printBB(BitBoard bb)
{   for(int i=7; i>=0; i--)          //top left to bottom right
    {   for(int j=0; j<8;j++)
            printf("%c ", bb & coordToBB(i,j) ? '1' : '.');
        puts("");
    }
    puts("");
}

