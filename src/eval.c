#include<stdio.h>
#include<stdlib.h>

#include "board.h"

const int32_t QUEEN = 800, ROOK = 500, BISHOP = 330, KNIGHT = 300, PAWN = 100; //quantify material

int32_t eval(Board b)
{   int32_t material = 0;
    pieceList pl = b.white;


    if(pl.pieces & Q)
        material += QUEEN * population(pl.bbList[1]);
    if(pl.pieces & R)
        material += ROOK * population(pl.bbList[2]);
    if(pl.pieces & B)
        material += BISHOP * population(pl.bbList[3]);
    if(pl.pieces & N)
        material += KNIGHT * population(pl.bbList[4]);
    if(pl.pieces & P)
        material += PAWN * population(pl.bbList[5]);
    pl = b.black;
    if(pl.pieces & Q)
        material -= QUEEN * population(pl.bbList[1]);
    if(pl.pieces & Q)
        material -= ROOK * population(pl.bbList[2]);
    if(pl.pieces & Q)
        material -= BISHOP * population(pl.bbList[3]);
    if(pl.pieces & Q)
        material -= KNIGHT * population(pl.bbList[4]);
    if(pl.pieces & Q)
        material -= PAWN * population(pl.bbList[5]);
    
    return material;
}


int main()
{   
}
