#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
//#inclued<>


//------------ASCII definitions--------

#define WKING 'K'
#define WQUEEN 'Q'
#define WROOK 'R'
#define WBISHOP 'B'
#define WKNIGHT 'N'
#define WPAWN 'P'
#define BKING 'k'
#define BQUEEN 'q'
#define BROOK 'r'
#define BBISHOP 'b'
#define BKNIGHT 'n'
#define BPAWN 'p'
#define ENPASSANT 'X'

#define EMPTY '.'

//-------------data indexes----------------

#define WHITEMOVE 
#define WHITEPIECE(index) square & 0x7F
//-------


#define OUTOFBOUNDS(square) square & 0x88  //10001000

#define square uint8_t
#define move uint16_t


typedef struct Board
{   square white[128];
    square black[128]
}Board;

_Bool isWhileMove(Board *b)
{   
    return b->white[]
}

void board_init(Board *b){b = calloc(1, sizeof(Board));}

void pawnMoves(move *moves, int *n, Board *b)
{   _Bool iswhitemove = isWhiteMove(b);
}
void kingMoves(move *moves, int *n, Board *b){}
void queenMoves(move *moves, int *n, Board *b){}
void rookMoves(move *moves, int *n, Board *b){}
void bishopMoves(move *moves, int *n, Board *b){}
void knightMoves(move *moves, int *n, Board *b){}

void moves(move *moves, int *n, Board *b)
{
}

void printBoard(Board b)
{   for(int i=112; i >= 0; i-= 16)
        printf("%.*s\n",8, &b.board[i]);
}
Board fenToBoard(char *fen)
{   Board b;
    board_init(&b);
    for(int i=112; i>=0; i++, fen++)
    {   printf("i:%d\n", i);
        switch(*fen)
        {   
        case WKING:
        case WQUEEN:
        case WROOK:
        case WBISHOP:
        case WKNIGHT:
        case WPAWN:
        case BKING:
        case BQUEEN:
        case BROOK:
        case BBISHOP:
        case BKNIGHT:
        case BPAWN:
            b.board[i] = *fen;
            break;
        case '1'...'8':
            for(int j = 0; j < *fen -48; j++)
                b.board[i+j] = EMPTY;
            i += *fen - 49;
            break;

        case '/': // '\'?
            i -= 25;
            break;
        default:
            puts("E 1");
            return b;
        }
    }
    puts("Sucess!\n");
    return b;
}
int main()
{   char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/";
    puts("1");
    Board board = fenToBoard(fen);
    puts("9");
    printBoard(board);
    return 0;
}
