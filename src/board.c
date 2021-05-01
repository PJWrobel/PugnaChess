/*
* Chess Engine Version 0.0.1
* Bitboard Layout
*
*/


#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define BitBoard uint64_t


//------------------------BitEnum-----------------------------

#define BitEnum char

//assign pieces to bits
#define K 1
#define P 2
#define Q 4
#define R 8
#define B 16
#define N 32
#define D 64

const BitEnum LOG2[65] = { [K] = 1, [P] = 2, [Q] = 3, [R] = 4, [B] = 5, [N] = 6, [D] = 7 };   //log2 table for single bit BitEnums

BitEnum charToBitEnum(char c) //piece character to corresponding BitEnum
{   c |= 0x20; //make all chars uppercase (all non-chars removed prior)
    switch(c)
    {   case 'K':
            return K;
        case 'Q':
            return Q;
        case 'R':
            return R;
        case 'B':
            return B;
        case 'N':
            return N;
        case 'P':
            return P;
        case 'D':
            return D;
        default: return -1; //TODO: ERR
    }
}
//------------------------------------------------------------

struct pieceList
{   BitEnum pieces;
    BitBoard bitList[7];
};

BitBoard coordToBB(int row, int col){ return 1 << row*8 << col; } //BitBoard with bit set to 8x8 coords given

typedef struct Board
{   struct pieceList white, black;
    BitEnum state;
}Board;

int board_init(Board *b)
{   *b = (Board){ .white = {0, {0} }, .black = {0, {0} }, .state = 0 };
    return 0;
}

//uint64_t BoardToHash(Board b){}

void addPiece(struct pieceList *list, BitEnum type, BitBoard square)//list as param for color handling
{   list->pieces |= type;
    list->bitList[ LOG2[ type ]] |= square;
}

Board fenToBoard(char *fen) //i mean it works...
{   
    Board returnBoard;
    board_init(&returnBoard);
    for(long i=0x8000000000000000; 1; i >>= 1, fen++)
    {   
noInc:
        switch(*fen)
        {   case ' ':
                goto breakout;
            case '1'...'8':
                i >>= *fen - 49; //'0' == 48
            case 'K':
            case 'Q':
            case 'R':
            case 'B':
            case 'N':
            case 'P':
                returnBoard.white.pieces |= charToBitEnum(*fen);
                returnBoard.white.bitList[LOG2[charToBitEnum(*fen)]] |= i;
                break;
            case 'k':
            case 'q':
            case 'r':
            case 'b':
            case 'n':
            case 'p':
                returnBoard.black.pieces |= charToBitEnum(*fen);
                returnBoard.black.bitList[LOG2[charToBitEnum(*fen)]] |= i;
                break;
            case '/':
                fen++;
                goto noInc;
            default: 
                printf("ERROR: FEN FORMAT\n");
                return returnBoard;
        }
    }
breakout:
    return returnBoard;//placeholder
}

void printBoard(Board board)
{   char grid[8][8] = {0};

}

int main()
{   printf("%zu\n", sizeof(Board));
    return 0;
}
