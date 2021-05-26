/*
* board.c
* Bitboard Layout
*
* Bitboard orientation:  "b5" := ( x=1, y=4 )  
*/

#ifndef BITBOARD_H_
#define BITBOARD_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<stdbool.h>

#include "bitboard.h"

#define WHITEMOVE 1 
#define WK_CASTLE 2
#define WQ_CASTLE 4
#define BK_CASTLE 8
#define BQ_CASTLE 16

//------------------------BitEnum-----------------------------
#define BitEnum uint64_t

#define K 1
#define P 2
#define Q 4
#define R 8
#define B 16
#define N 32

const BitEnum LOG2[33];
BitEnum charToBitEnum(char c);
char BitEnumToChar(BitEnum be);
//-----------------------------------------------------------


typedef struct piece;
struct pieceList;
typedef struct Board;

int board_init(Board *b); //init clear board
void printPieces(struct pieceList pl);
//uint64_t BoardToHash(Board b){}
void addPiece(struct pieceList *list, BitEnum type, BitBoard square);//list as param for color handling
void printBoard(Board board, _Bool pretty);
Board fenToBoard(char *fen);
