/*
* board.c
* Bitboard Layout
*
*
* Bitboard orientation:  "b5" := ( x=1, y=4 )  
*/

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

//assign pieces to bits
#define K 1
#define P 2
#define Q 4
#define R 8
#define B 16
#define N 32

const BitEnum LOG2[33] = { [K] = 0, [P] = 1, [Q] = 2, [R] = 3, [B] = 4, [N] = 5 };   //log2 table for single bit BitEnums

BitEnum charToBitEnum(char c)           //piece character to corresponding BitEnum TODO: remake into dict?
{   c &= ~0x20;                         //make all chars uppercase (all non-chars removed prior)
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
        default: return -1; //TODO: ERR
    }
}
char BitEnumToChar(BitEnum be)
{   switch(be)
    {   case K:
            return 'K';
        case Q:
            return 'Q';
        case R:
            return 'R';
        case B:
            return 'B';
        case N:
            return 'N';
        case P:
            return 'P';
        default: return -1; //TODO: ERR
    }
}
//------------------------------------------------------------

typedef struct piece
{   BitEnum type;
    BitBoard square;
}piece;

struct pieceList
{   BitEnum pieces;
    BitBoard bbList[6];
};

typedef struct Position
{   struct pieceList white;
    struct pieceList black;
    BitBoard Wall;
    BitBoard Ball;
    BitEnum state;
    BitBoard passant;
}Position;

int board_init(Position *b) //init clear board
{   b = calloc(1, sizeof(Position));
    return 0;
}

void printPieces(struct pieceList pl)
{   for(int i = bitscan(pl.pieces); i; i = bitscan(pl.pieces))
    {   
        printf("%c:\n", BitEnumToChar(i));
        printBB(pl.bbList[LOG2[i]]);
        pl.pieces ^= i;
    }
}

//uint64_t BoardToHash(Position b){}

void addPiece(struct pieceList *list, BitEnum type, BitBoard square)//list as param for color handling
{   list->pieces |= type;
    list->bbList[ LOG2[ type ]] |= square;
}

piece* pieces(int *returnSize, struct pieceList list)//TODO: handle error: too many pieces (> 16 pieces)
{   *returnSize = 0;
    piece *retList = calloc(17, sizeof(piece));
    
    for(int i=0; i<6; i++)                          //sizeof(pieceList.bbList)
    {   for(BitBoard bb = list.bbList[i]; bb; *returnSize += 1)
        {   retList[*returnSize].type = bitscan(list.pieces);
            retList[*returnSize].square = bitscan(bb);
            printf("%c", BitEnumToChar(retList[*returnSize].type));
            bb ^= retList[*returnSize].square;
            if(!bb)
                list.pieces ^= retList[*returnSize].type;
        }
    }
    return retList;
}

void printBoard(Position board, _Bool pretty)
{   
    int WMat, BMat; //white and black material
    char grid[8][8];
    int *coords;
    
    piece *white = pieces(&WMat, board.white); //list of individual pieces of len WMat
    piece *black = pieces(&BMat, board.black);
    
    //printPieces(board.white); //print each bitboard
    //printf("Black:\n");
    //printPieces(board.black);
    
    memset(grid, ' ', 64);    //for proper print formatting: grid[8][8] = {' '}

    for(int i=0; i<WMat; i++)
    {   coords = BBToCoord(white[i].square);
        grid[ coords[1] ][ coords[0] ] = BitEnumToChar(white[i].type);
        free(coords);
    }
    for(int i=0; i<BMat; i++)
    {   coords = BBToCoord(black[i].square);
        grid[ coords[1] ][ coords[0] ] = 0x20 ^ BitEnumToChar(black[i].type);
        free(coords);
    }
    if(board.passant)
    {   coords = BBToCoord(board.passant);
        grid[coords[1]][coords[0]] = 'E';
        free(coords);
    }
    
    //-----------------printing-------------------
    printf("Move: %c Castle: %c%c%c%c\n", board.state & WHITEMOVE ? 'W' : 'B',
                                          board.state & WK_CASTLE ? 'K' : ' ',
                                          board.state & WQ_CASTLE ? 'Q' : ' ',
                                          board.state & BK_CASTLE ? 'k' : ' ',
                                          board.state & BQ_CASTLE ? 'q' : ' ');
    if(pretty)
    {   printf("| - + - + - + - + - + - + - + - |\n");
        for(int i=7;i>=0;i--)
        {   printf("|");
            for(int j=0; j<8; j++)
                printf(" %c |",grid[i][j]);
            printf("\n| - + - + - + - + - + - + - + - |\n");
        }
    }
    else
    {   for(int i=7;i>=0;i--)
        {   for(int j=0; j<8; j++)
                printf("%c",grid[i][j]);
            printf("\n");
        }
    }
}

Position fenToBoard(char *fen)
{   
    Position returnBoard;
    board_init(&returnBoard);
    BitBoard mask;

    for(int i = 7; i >= 0; i--)
    {   for(int j = 0; j < 9; j++, fen++)
        {   
            mask = coordToBB(i,j);
            switch(*fen)
            {   
            case '1'...'8':
                j += *fen -49;
                break;
            case 'K':
            case 'Q':
            case 'R':
            case 'B':
            case 'N':
            case 'P':
                addPiece(&returnBoard.white, charToBitEnum(*fen), mask);
                returnBoard.Wall |= mask;
                break;
            case 'k':
            case 'q':
            case 'r':
            case 'b':
            case 'n':
            case 'p':
                addPiece(&returnBoard.black, charToBitEnum(*fen), mask);
                returnBoard.Ball |= mask;
                break;
            case '/':
                if(j == 8)
                    break;
                //else continue to default
            case ' ':
                if(i==0 && j==8)
                    break; //else default
            default: 
                printf("ERROR FEN: (%d,%d):%c\n",i,j,*fen);
                return returnBoard;
            }
        }
    }

    if(*fen == 'w')
        returnBoard.state |= WHITEMOVE;
    else if(*fen == 'b')
        returnBoard.state &= ~WHITEMOVE;
    else{puts("E1"); return returnBoard;}
    fen++;
    if(*fen != ' '){puts("E2"); return returnBoard;}
    fen++;
    for(int i=0; i<4; i++, fen++)
    {   switch(*fen)
        {
        case 'K':
            returnBoard.state |= WK_CASTLE;
            break;
        case 'Q':
            returnBoard.state |= WQ_CASTLE;
            break;
        case 'k':
            returnBoard.state |= BK_CASTLE;
            break;
        case 'q':
            returnBoard.state |= BQ_CASTLE;
            break;
        case '-':
            if(i){puts("E3"); return returnBoard;}
            i = 4;
            fen++;
            if(*fen != ' '){puts("E4"); return returnBoard;}
            fen--;
            break;
        case ' ':
            if(!i){puts("E5"); return returnBoard;}
            i = 4;
            break;
        }
    }
    fen++;
    switch(*fen) //en passant square, must be in rank 3 (white) or 6 (black)
    {   case '-':
            break;
        case 'a'...'h':
            fen++;
            if(*fen == '3' || *fen == '6')
                returnBoard.passant = coordToBB(*fen - 49, *(fen-1) - 97);//TODO.
            
            else{puts("E6"); return returnBoard;}
            break;
        default:
            printf("E7: %s\n", fen);
            return returnBoard;
    }
    fen++;
    if(*fen != ' '){puts("E8"); return returnBoard;}

    fen++;
    switch(*fen) //not doing 50 move rule, no atoi so fen is properly placed following
    {   case '0'...'9':
            fen++;
            if(*fen == ' ')
                break;
            fen++; //if num is 2-digit
            if(*fen == ' ')
                break;
            //else continue;
        default:
            puts("E9");
            return returnBoard;
    }
    fen++;
    //int moves = atoi(fen);
    puts("Successfully loaded FEN");
    return returnBoard;
}

int main(int argc, char *argv[])
{   char *fen = "r1bqkb1r/pppp1ppp/2Bn4/4p3/3P4/5N2/PPPP1PPP/RNBQ1RK1 w - - 0 11";
    Position board = fenToBoard(fen);
    printBoard(board,true);

    printf("%zu\n", sizeof(Position));
    return 0;
}
