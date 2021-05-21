#include<stdio.h>
#include<stdlib.h>
//#include<>
//#include<>
//#include<>
#include "bitboard.h"

#define LINE_N  0x0101010101010101  //1s on file a
#define LINE_E  0xF                 //1s on rank 1
#define LINE_NE 0x8040201008040201  //a1 to h8
#define LINE_NW 0x0102040810204080  //a8 to h1

BitBoard all(pieceList *l)//all pieces for one side
{   BitBoard ret=0;
    
    ret |= l->bbList[0]//K
    ret |= l->bbList[1]//P
    ret |= l->bbList[2]//Q
    ret |= l->bbList[3]//R
    ret |= l->bbList[4]//B
    ret |= l->bbList[5]//N
    return ret;
}
BitBoard rookAtk(BitBoard bb)
{   
}

int main()//testing
{   
}
