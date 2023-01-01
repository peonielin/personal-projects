// CS_frogger
//
// This program was written by [Peonie Lin] (z5454933)
// on [13/10/2022]
//
// TODO: Allows u to play cs_frogger!

#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  CONSTANTS  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided constants
#define SIZE        9
#define TRUE        1
#define FALSE       0
#define RIGHT       1
#define LEFT        -1

// TODO: you may choose to add additional #defines here.

#define LIVES 3

// Provided Enums
enum tile_type {LILLYPAD, BANK, WATER, TURTLE, LOG};

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  STRUCTS  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided structs
struct board_tile {
    enum tile_type type; 
    int occupied; 
    int bug_assign;     
};

struct frog {
    int row;
    int col;   
};

struct bugs {
    int dir;  
};

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  FUNCTION PROTOTYPES  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Your function prototypes here
void add_turtles(struct board_tile game_board[SIZE][SIZE], int turtle_coord_x, int turtle_coord_y);
int log_end_boundary(int *log_end_function);
int log_start_boundary(int *log_start_function);
int log_row_boundary(struct board_tile board[SIZE][SIZE], int log_row_function);
void log_row_print(struct board_tile board[SIZE][SIZE], int *log_row_function, int log_end_function, 
    int log_start_function);
void clear_row(struct board_tile board[SIZE][SIZE], int num_row_cleared, int frog_row);
int find_col_start(struct board_tile board[SIZE][SIZE], int remove_log_row, int remove_col_start);
int find_col_end(struct board_tile board[SIZE][SIZE], int remove_log_row, int remove_col_end);
void set_log(struct board_tile board[SIZE][SIZE], int col_start, int col_end, int remove_log_row);
void remove_log(struct board_tile board[SIZE][SIZE],int remove_log_row,
    int remove_log_col, int remove_col_start, int remove_col_end);
//int bug_movement(struct board_tile board[SIZE][SIZE], int bug_direction);
struct frog move_frog(struct frog froggo, struct board_tile  game_board[SIZE][SIZE], int x_dir, int y_dir);
struct frog die(struct frog froggo, struct board_tile board[SIZE][SIZE], int *lives_left);
void add_bug(struct frog froggo, struct bugs bug[SIZE][SIZE], struct board_tile game_board[SIZE][SIZE], int row, int col);
void move_bug(struct bugs bug[SIZE][SIZE], struct board_tile game_board[SIZE][SIZE], int *row, int *col);
void print_win_message();
void print_lose_message();


// Prints out the current state of the board.
void print_board(struct board_tile board[SIZE][SIZE]);
char type_to_char(enum tile_type type);
void initialise_game_board(struct board_tile board[SIZE][SIZE]);


////////////////////////////////////////////////////////////////////////////////
//////////////////////////  FUNCTION IMPLEMENTATIONS  //////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {

    printf("Welcome to CSE Frogger!\n");
    struct board_tile game_board[SIZE][SIZE];
    struct bugs bug[SIZE][SIZE];


    // TODO (Stage 1.1) Initialise the gameboard.
    initialise_game_board(game_board);
    // Read user input and place turtles.
    printf("How many turtles? ");
    int num_turtles;
    scanf("%d", &num_turtles);
    if (num_turtles > 0) {
        printf("Enter pairs:\n");
        int turtle_coord_x;
        int turtle_coord_y;
        //as many scanf as num_turtles  
        for (int i = 1; i <= num_turtles; i++) {
            scanf("%d %d", &turtle_coord_x, &turtle_coord_y);
            add_turtles(game_board, turtle_coord_x, turtle_coord_y);                
        }
    }                 
    {
    
    }
    // Start the game and print out the gameboard.
    printf("Game Started\n");
    print_board(game_board);

    printf("Enter command: ");
    // TODO (Stage 1.3): Create a command loop, to read and execute commands!
    
    struct frog frog;
    frog.row = 8;
    frog.col = 4;

    char input_command;
    int lives_left = LIVES;

    while (scanf(" %c", &input_command) == 1) {

        if (input_command == 'l') {
            int log_row;
            int log_start;
            int log_end;
            scanf("%d %d %d", &log_row, &log_start, &log_end);
            log_start_boundary(&log_start);
            log_end_boundary(&log_end);
            log_row_boundary(game_board, log_row); 
            log_row_print(game_board, &log_row, log_end, log_start);

        } else if (input_command == 'c') {
            int num_row_cleared;
            scanf("%d", &num_row_cleared);
            clear_row(game_board, num_row_cleared, frog.row);

        } else if (input_command == 'r') {
            int remove_log_row;
            int remove_log_col;
            scanf(" %d %d", &remove_log_row, &remove_log_col);
            int remove_col_start = remove_log_col;
            int remove_col_end = remove_log_col;
            remove_log(game_board, remove_log_row, remove_log_col, remove_col_start, 
            remove_col_end);
                
        } else if (input_command == 'b') {
            int new_row;
            int new_col;
            scanf("%d %d", &new_row, &new_col);
            add_bug(frog, bug, game_board, new_row, new_col);
        } else if (input_command == 'w') {
            frog = move_frog(frog, game_board, 0, -1);
            move_bug(bug,game_board, &new_row, &new_col);
        } else if (input_command == 'a') {
            frog = move_frog(frog, game_board, -1, 0);
            move_bug(bug,game_board, &new_row,  &new_col);
        } else if (input_command == 's' ) { 
            frog = move_frog(frog, game_board, 0, 1);
            move_bug(bug,game_board, &new_row,  &new_col);
        } else if (input_command == 'd') { 
            frog = move_frog(frog, game_board, 1, 0);
            move_bug(bug,game_board, &new_row,  &new_col);
        } 
         
        print_board(game_board);
        
        if (game_board[frog.row][frog.col].occupied == TRUE 
            && game_board[frog.row][frog.col].type == LILLYPAD) {
            print_win_message();
        }

        if (game_board[frog.row][frog.col].occupied == TRUE && 
        (game_board[frog.row][frog.col].type == WATER
        || game_board[frog.row][frog.col].bug_assign == TRUE)
        && lives_left <= 3 && lives_left >= 1 ) {
            if (lives_left == 1) {
                print_lose_message();
            }
            frog = die(frog, game_board, &lives_left);
            printf("\n# LIVES LEFT: %d #\n\n", lives_left);
            print_board(game_board);
        }       
    
        printf("Enter command: ");

    }
    printf("Thank you for playing CSE Frogger!\n");
    return 0;
}



////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ADDITIONAL FUNCTIONS /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Add more functions here!

void add_turtles(struct board_tile game_board[SIZE][SIZE], 
    int turtle_coord_x, int turtle_coord_y) {
    if (turtle_coord_x <= 7 && turtle_coord_x >= 1) {
        if (turtle_coord_y <= 8 && turtle_coord_y >= 0) {
            game_board[turtle_coord_x][turtle_coord_y].type = TURTLE;
        }
    }
}

void print_win_message() {
    printf("\nWahoo!! You Won!\n\n");
    printf("Thank you for playing CSE Frogger!\n");
    exit(0);
}
void print_lose_message() {
    printf("\n !! GAME OVER !!\n\n");
    printf("Thank you for playing CSE Frogger!\n");
    exit(0);
}

void add_bug(struct frog froggo, struct bugs bug[SIZE][SIZE], 
    struct board_tile game_board[SIZE][SIZE],
    int row, int col) {
    if (row <= SIZE - 2 && row >= 1 && col <= SIZE - 1) {
        if (col >= 0  && !(col == froggo.col && row == froggo.row)) {
            if ((game_board[row][col].type == TURTLE || 
                game_board[row][col].type == LOG) ) {
                bug[row][col].dir = RIGHT;
                game_board[row][col].bug_assign = TRUE; 
            }
        }      
    }
}

void move_bug(struct bugs bug[SIZE][SIZE], struct board_tile game_board[SIZE][SIZE], int *row, int *col) {
    int new_col = *col + bug[*row][*col].dir;
    //decide direction
    if (new_col >= SIZE || new_col < 0 || game_board[*row][new_col].type == WATER 
     || game_board[*row][new_col].bug_assign == TRUE ){
        bug[*row][*col].dir *= -1;
        new_col = new_col + bug[*row][*col].dir;
    } 
    // verify new movment
    if (new_col >= 0 && new_col <= SIZE - 1 && 
        game_board[*row][new_col].type != WATER) {
        game_board[*row][*col].bug_assign = FALSE;
        *col += bug[*row][*col].dir;
        game_board[*row][*col].bug_assign = TRUE;
    }

}

struct frog move_frog(struct frog froggo, struct board_tile game_board[SIZE][SIZE], 
    int x_dir, int y_dir) {
    if (froggo.row + y_dir >= 0 && froggo.row + y_dir < SIZE 
        && froggo.col + x_dir >= 0 && froggo.col + x_dir < SIZE ) {
        game_board[froggo.row][froggo.col].occupied = FALSE;
        froggo.row += y_dir;
        froggo.col += x_dir;
        game_board[froggo.row][froggo.col].occupied = TRUE;
    }
    return froggo;
}

struct frog die(struct frog froggo, struct board_tile board[SIZE][SIZE],
    int *lives_left) {
    *lives_left = *lives_left - 1;
    board[froggo.row][froggo.col].occupied = FALSE;
    froggo.row = 8;
    froggo.col = 4;
    board[froggo.row][froggo.col].occupied = TRUE;
    return froggo;
}

void initialise_game_board(struct board_tile board[SIZE][SIZE]) {
    
    for (int i = 0; i <= 8; i = i + 2) {
        board[0][i].type = LILLYPAD;
        board[0][i].occupied = FALSE;
    }

    for (int i = 1; i <= 7; i = i + 2) {
        board[0][i].type = WATER;
        board[0][i].occupied = FALSE;
    }

    for (int i = 1; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
            board[i][j].type = WATER;
            board[i][j].occupied = FALSE;
        }
    }

    for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
            board[i][j].bug_assign = FALSE;
        
        }
        for(int j = 0; j <= 8; j++) {
            board[8][j].type = BANK;
            if (j == 4) {
                board[8][j].occupied = TRUE;
            }
            else {
                board[8][j].occupied = FALSE;           
            }
        }

    }
    
}
    

int log_end_boundary(int *log_end_function) {
    if (*log_end_function > 8) {
        *log_end_function = 8;
    }
    return *log_end_function;
}

int log_start_boundary(int *log_start_function) {
    if (*log_start_function < 0) {
        *log_start_function = 0;
    }
    return *log_start_function;
}

int log_row_boundary(struct board_tile board[SIZE][SIZE], int log_row_function) {
    for (int j = 0; j < SIZE; j++) {
        if (board[log_row_function][j].type == TURTLE) {
            return 1;
        }  
    } 
    return 0;
}

void log_row_print(struct board_tile board[SIZE][SIZE], int *log_row_function, 
    int log_end_function, int log_start_function) {
    if (log_row_boundary(board, *log_row_function) == 0 && log_end_function <= 8 && 
        log_end_function >= 1) {
        for(int i = log_start_function; i <= log_end_function; i++) {
            board[*log_row_function][i].type = LOG;
        }

    }
    
}

void clear_row(struct board_tile board[SIZE][SIZE], int num_row_cleared, int frog_row) {
    if (num_row_cleared != frog_row && num_row_cleared >= 1 && num_row_cleared <= 7) {
        for(int i = 0; i <= 8; i++) {
            board[num_row_cleared][i].type = WATER;
            board[num_row_cleared][i].bug_assign = FALSE;
        }
    }
    else if (num_row_cleared <= 1 || num_row_cleared >= 7) {}
}

int find_col_start(struct board_tile board[SIZE][SIZE], int remove_log_row, 
    int remove_col_start) {
    while (board[remove_log_row][remove_col_start].type == LOG && remove_col_start > 0) {
        remove_col_start--;
    } int col_start = remove_col_start;
    return col_start;
}

int find_col_end(struct board_tile board[SIZE][SIZE], int remove_log_row,
    int remove_col_end) {
    while (board[remove_log_row][remove_col_end].type == LOG && remove_col_end < 8) {
        remove_col_end++;
    }
    return remove_col_end;
}

void set_log(struct board_tile board[SIZE][SIZE], int col_start, int col_end, 
    int remove_log_row) {
    if (col_start >= 0 && col_end <= 8) {
        for (int i = col_start; i <= col_end; i++) {
            board[remove_log_row][i].type = WATER;
        }
    }
}


void remove_log(struct board_tile board[SIZE][SIZE], int remove_log_row,
    int remove_log_col, int remove_col_start, int remove_col_end) {
    if (remove_log_row >= 1 && remove_log_row <= 7 && remove_log_col >= 0) {
        if (remove_log_col <= 8 && board[remove_log_row][remove_log_row].type == LOG) {
            int col_start = find_col_start(board, remove_log_row, remove_col_start);
            int col_end = find_col_end(board, remove_log_row, remove_col_end);
            set_log(board, col_start, col_end, remove_log_row);
        }
    }

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void print_board(struct board_tile board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            char type_char = '\0';
            if (board[row][col].occupied) {
                type_char = 'F';
            } else if (board[row][col].bug_assign) {
                type_char = 'B';
            } else {
                type_char = type_to_char(board[row][col].type);
            }

            printf("%c ", type_char);
        }
        printf("\n");
    }
}

char type_to_char(enum tile_type type) {
    char type_char = ' ';
    if (type == LILLYPAD) {
        type_char = 'o';
    } else if (type == BANK) {
        type_char = 'x';
    } else if (type == WATER) {
        type_char = '~';
    } else if (type == TURTLE) {
        type_char = 'T';
    } else if (type == LOG) {
        type_char = 'L';
    }
    return type_char;
}