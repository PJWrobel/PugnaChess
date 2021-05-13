/*
* board.c
* Bitboard Layout
*
* Bitboard orientation:  "b5" := ( x=1, y=4 )  
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#define bitscan(bb) (bb & -bb)                      //you know it
#define BitBoard uint64_t

#define WHITEMOVE 1
#define BLACKMOVE 
#define WK_CASTLE 2
#define WQ_CASTLE 4
#define BK_CASTLE 8
#define BQ_CASTLE 16
//#define 
//#define
//#define

//------------------------BitEnum-----------------------------

#define BitEnum uint64_t

//assign pieces to bits
#define K 1
#define P 2
#define Q 4
#define R 8
#define B 16
#define N 32

const BitEnum LOG2[33] = { [K] = 1, [P] = 2, [Q] = 3, [R] = 4, [B] = 5, [N] = 6 };   //log2 table for single bit BitEnums

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
    BitBoard bbList[7];
}pieceList;

typedef struct Board
{   struct pieceList white;
    struct pieceList black;
    BitBoard Wall;
    BitBoard Ball;
    BitEnum state;
    BitBoard passant;
}Board;

int board_init(Board *b) //init clear board
{   *b = (Board){ .white = {0, {0} }, .black = {0, {0} }, .Wall = 0, .Ball = 0, .state = 0, .passant = 0 };
    return 0;
}

BitBoard coordToBB(int row, int col)
{   BitBoard bb = 1;
    bb <<= row*8 + col;
    return bb;
}      //bitboard mask for 8x8 coords

uint64_t bitscanRemove(uint64_t *bb) //either BB or BE
{   BitBoard ret = bitscan(*bb);
    *bb ^= ret;
    return ret;
}

void printBB(BitBoard bb)                           
{   BitBoard mask;
    for(int i=7; i>=0; i--)          //top left to bottom right
    {   for(int j=0; j<8;j++)
            printf("%c", bb & coordToBB(i,j) ? 'X' : 'O');
        puts("");
    }
}

int* BBToCoord(BitBoard bb)
{   int rank, file;
    BitBoard mask = 0x101010101010101;             //1's on a column
    for(file = 0; ! (mask & bb); file++, bb >>= 1); //shift board left until piece in a column
    for(rank = 0; bb; rank++, bb >>= 8);
    int *coords = malloc(8); //TODO: free?
    coords[0] = file;
    coords[1] = rank-1;
    return coords;
}

void printPieces(struct pieceList pl)
{   for(int i = bitscan(pl.pieces); i; i = bitscan(pl.pieces))
    {   
        printf("%c:\n", BitEnumToChar(i));
        printBB(pl.bbList[LOG2[i]]);
        pl.pieces ^= i;
    }
}

//uint64_t BoardToHash(Board b){}

void addPiece(struct pieceList *list, BitEnum type, BitBoard square)//list as param for color handling
{   list->pieces |= type;
    list->bbList[ LOG2[ type ]] |= square;
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
{   
    int WMat, BMat; //white and black material
    char grid[8][8];

    piece *white = pieces(&WMat, board.white); //list of individual pieces of len WMat
    piece *black = pieces(&BMat, board.black);
    
    //printPieces(board.white); //print each bitboard
    //printf("Black:\n");
    //printPieces(board.black);
    
    memset(grid, ' ', 64);    //for proper print formatting: grid[8][8] = {' '}

    for(int i=0; i<WMat; i++)
    {   int *coords = BBToCoord(white[i].square);
        grid[ coords[1] ][ coords[0] ] = BitEnumToChar(white[i].type);
        free(coords);
    }
    for(int i=0; i<BMat; i++)
    {   int *coords = BBToCoord(black[i].square);
        grid[ coords[1] ][ coords[0] ] = 0x20 ^ BitEnumToChar(black[i].type);
        free(coords);
    }
    for(int i=7;i>=0;i--)
    {   for(int j=0; j<8; j++)
            printf("%c",grid[i][j]);
        printf("\n");
    }
}

Board fenToBoard(char *fen)
{   
    Board returnBoard;
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
        returnBoard.state ^= WHITEMOVE;
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
        case 'k':
            returnBoard.state |= BK_CASTLE;
        case 'q':
            returnBoard.state |= BQ_CASTLE;
        case '-':
            if(i){puts("E3"); return returnBoard;}
            i = 4;
            fen++;
            if(*fen != ' '){puts("E4"); return returnBoard;}
            break;
        case ' ':
            if(!i){puts("E5"); return returnBoard;}
            i = 4;
        }
    }
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
            puts("E7");
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
    int moves = atoi(fen);
    return returnBoard;
}

int main()
{   char *fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e3 0 1";
    Board board = fenToBoard(fen);
    printBB(board.Wall);
    printBoard(board);

    printf("%zu\n", sizeof(Board));
    return 0;
}
