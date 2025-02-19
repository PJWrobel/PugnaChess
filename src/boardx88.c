#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<time.h>

#define u8 uint8_t
#define i8 int8_t
#define u32 uint32_t
#define u64 uint64_t

#define C_TO_I(R, F) ((-R+8)*16 + F-1)

#define AT(R,F) x88[ C_TO_I(R,F) ]

const i8 knight_moves[] = {33, -33, 31, -31, 18, -18, 14, -14};
const i8 bishop_moves[] = {17, -17, 15, -15};
const i8 rook_moves[]   = {16, -16, 1, -1};
const i8 queen_moves[]  = {-17, -16, -15, -1, 1, 15, 16, 17};
const i8 w_pawn_moves[] = {15, 17, 16, 32};
const i8 b_pawn_moves[] = {-15, -17, -16, -32};


typedef enum __attribute__((__packed__)) Piece //white pieces are upper case (if piece ^ 32, is (black*?))
{
    Blank = ' ',
    W_King = 'K',
    B_King = 'k',
    W_Queen = 'Q',
    B_Queen = 'q',
    W_Rook = 'R',
    B_Rook = 'r',
    W_Bishop = 'B',
    B_Bishop = 'b',
    W_Knight = 'K',
    B_Knight = 'k',
    W_Pawn = 'P',
    B_Pawn = 'p'
} Piece;

typedef struct BoardObjects
{
    u64 row8;
    u64 ply;
    u64 row7;
    u64 state_enum;
    u64 row6;
    u64 PAD_0;
    u64 row5;
    u64 PAD_1;
    u64 row4;
    u64 PAD_2;
    u64 row3;
    u64 PAD_3;
    u64 row2;
    u64 PAD_4;
    u64 row1;
    u64 PAD_5;
}BoardObjects;

typedef union Board
{   BoardObjects object;
    Piece x88[128];
}Board;

Board* board_new()
{
    return calloc(sizeof(Board), 1);
}

void print_board(Board b)
{
    for(int rank=8; rank>=1; rank--) 
        printf("%.8s\n", (char*)&b.AT(rank,1));
}

void print_board_pretty(Board b)
{
    printf("+ - + - + - + - + - + - + - + - +\n");
    for(int rank=8; rank>=1; rank--) {
        printf("|");
        for(int file=1; file<=8; file++) {
            printf(" %c |", b.AT(rank,file));
        }
        printf("\n+ - + - + - + - + - + - + - + - +\n");
    }
}

Board board_from_fen(const char* const_fen) //possible const string, must memcpy
{
    char** split(char* str, char delim, int *retSize)
    {   char *nextStart = str;
        char **retArr = malloc(20*sizeof(char*)); //free?
        int i;
        for(i = 0; *str; str++)
        {   
            if(*str == delim)
            {
                retArr[i++] = nextStart;
                nextStart = str + 1;
                *str = '\0';
            }
        }
        retArr[i++] = nextStart;
        *retSize = i;
        return retArr;
    }

    Board b = {~0};

    char* fen = malloc(100); //longest possible fen(+1)?
    memcpy(fen, const_fen, 100); //TODO: determine length of const_fen
    
    int n;
    
    char** split_fen = split(fen, ' ', &n);
    
    if(n != 6){
        puts("FEN Error: incorrect number of fields");
        exit(1);
    }
    
    char** split_board = split(split_fen[0], '/', &n);
    
    if(n != 8){
        puts("FEN Error: incorrect number of rows");
        exit(1);
    }
    /*for(int i=0; i<n; i++) { //testing
        printf("%s\n", split_board[i]);
    }*/
    
    for(int row=8; row>=1; row--)
    {   int col = 1;
        int split_index = -row + 8; //fen field [0] -> 8th row
        for(char* head = split_board[split_index]; col <= 8; head++)
        {   char piece = *head;
            switch(piece)
            {   case '1'...'8':
                    for(int blanks = piece -'0'; blanks != 0;col++, blanks--) {
                        b.x88[C_TO_I(row,col)] = ' ';
                    }
                    break;
                case 'k':
                case 'q':
                case 'r':
                case 'b':
                case 'n':
                case 'p':
                case 'K':
                case 'Q':
                case 'R':
                case 'B':
                case 'N':
                case 'P':
                    b.x88[C_TO_I(row,col)] = piece;
                    col++;
                    break;
                default:
                    fprintf(stderr, "FEN board layout error: \'%c\', r:%d c:%d", piece, row, col);
                    exit(1);
            }
            //printf("c:%d\n", col);
        }
    }
    return b;
}

void move(Board *b,int from, int move) {
    int move_square = b->x88[from] + move;
    if (b->x88[move_square])

}

int main()
{
    Board b;
    b.object = (BoardObjects) {
        *(u64*)"rnbqkbnr", ~0,
        *(u64*)"pppppppp", ~0,
        *(u64*)"        ", ~0,
        *(u64*)"        ", ~0,
        *(u64*)"        ", ~0,
        *(u64*)"        ", ~0,
        *(u64*)"PPPPPPPP", ~0,
        *(u64*)"RNBQKBNR", ~0
    };

    //print_board(b);

    b = board_from_fen("r1bqkb1r/pppp1ppp/2n2n2/4p1N1/2B1P3/8/PPPP1PPP/RNBQ1RK1 w - - 0 11");

    print_board_pretty(b);

}

