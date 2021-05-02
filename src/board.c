/*
* Chess Engine Version 0.0.1
* Bitboard Layout
*
*/


#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define BitBoard uint64_t
#define bitscan(bb) (bb & -bb)                              //bitboard bitscan
#define coordToBB(row,col) (1 << row*8 << col)        //bitboard mask for 8x8 coords 

void printBB(BitBoard bb)
{   BitBoard mask = 0x8000000000000000;
    for(int i=0; i<8; i++)
    {   for(int j=0; j<8; j++, mask >>= 1)
            printf("%c", bb & mask ? '1' : 'o');
        printf("\n");
    }
}

int* BBToCoord(BitBoard bb)
{   int rank, file;
    BitBoard mask = 0x8080808080808080; //1's on h column 
    for(file = 0; ! (mask & bb); file++, bb <<= 1); //left shift to go right
    for(rank = 0; bb; rank++, bb >>= 8);
    int *coords = malloc(8);
    coords[0] = 7-file;
    coords[1] = rank-1;
    return coords;
}

//------------------------BitEnum-----------------------------

#define BitEnum uint64_t

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
{   c &= ~0x20; //make all chars uppercase (all non-chars removed prior)
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
        case D:
            return 'D';
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
    BitBoard bbList[7];
}pieceList;


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
    list->bbList[ LOG2[ type ]] |= square;
}

Board fenToBoard(char *fen) //i mean it works...
{   
    Board returnBoard;
    board_init(&returnBoard);
    BitBoard mask = 0x8000000000000000;
    for(int j=0; mask && j < 9; mask >>= 1, fen++, j++)
    {   
noInc:
        switch(*fen)
        {   case ' ':
                goto breakout;
            case '1'...'8':
                mask >>= *fen -49; //'0' == 48
                j += *fen -49;
                break;
            case 'K':
            case 'Q':
            case 'R':
            case 'B':
            case 'N':
            case 'P':
                returnBoard.white.pieces |= charToBitEnum(*fen);
                returnBoard.white.bbList[LOG2[charToBitEnum(*fen)]] |= mask;
                break;
            case 'k':
            case 'q':
            case 'r':
            case 'b':
            case 'n':
            case 'p':
                returnBoard.black.pieces |= charToBitEnum(*fen);
                returnBoard.black.bbList[LOG2[charToBitEnum(*fen)]] |= mask;
                break;
            case '/':
                if(j != 8){
                    //TODO: ERR
                    printf("ERROR\n");
                    return returnBoard;
                }
                fen++;
                j = 0;
                goto noInc;
            default: 
                printf("ERROR: FEN FORMAT\n");
                return returnBoard;
        }
    }
    //TODO: ERROR i == 0
breakout:
    puts("Sucess!");
    return returnBoard;//placeholder
}

piece* pieces(int *returnSize, struct pieceList list)
{   *returnSize = 0;
    piece *retList = calloc(16, sizeof(piece));
    for(int i=0; i<7; i++)                          //sizeof(pieceList.bbList)
    {   for(BitBoard bb = list.bbList[i]; bb; *returnSize += 1)
        {   retList[*returnSize].type = bitscan(list.pieces);
            retList[*returnSize].square = bitscan(bb);
            bb ^= retList[*returnSize].square;
            if(!bb)
                list.pieces ^= retList[*returnSize].type;
        }
    }
    return retList;
}
void printBoard(Board board)
{   char grid[8][8] = {0};
    int WMat, BMat; //white and black material
    piece *white = pieces(&WMat, board.white);
    piece *black = pieces(&BMat, board.black);
    for(int i=0; i<WMat; i++)
    {   int *coords = BBToCoord(white[i].square);
        grid[ coords[1] ][ coords[0] ] = BitEnumToChar(white[i].type);
        free(coords);
    }
    for(int i=0; i<BMat; i++)
    {   int *coords = BBToCoord(black[i].square);
        grid[ coords[1] ][ coords[0] ] = 0x20 ^ BitEnumToChar(black[i].type);
    }
    for(int i=0;i<8;i++)
    {   for(int j=0; j<8; j++)
            printf("%c",grid[i][j]);
        printf("\n");
    }
}

int main()
{   char *fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1";
    Board board = fenToBoard(fen);
    printBoard(board);
    
    //printf("%zu\n", sizeof(Board));
    return 0;
}
