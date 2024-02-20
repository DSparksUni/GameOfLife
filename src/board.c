#include "board.h"

uni_board* uni_board_init(size_t r, size_t c, bool* out_error) {
    //| Allocates and initializes a board instance
    //|
    //| Arguments:
    //|     r: Number of rows
    //|     c: Number of columns
    //|     out_error: Error state (UNI_SUCCESS upon no error)
    //|
    //| Returns:
    //|     Initialized board

    uni_board* this = (uni_board*)malloc(sizeof(uni_board));

    this->board = (bool*)malloc(r*c);
    memset(this->board, false, r*c);
    this->rows = r;
    this->cols = c;
    this->size = r*c;

    *out_error = false;
    return this;
}

void uni_board_randomize(uni_board* this) {
    //Randomizes each cell of the given board

    for(size_t i = 0; i < this->size; i++) {
        const bool rng = rand() % 2;
        this->board[i] = rng;
    }
}

bool uni_board_get(uni_board* this, size_t row, size_t col) {
    //Basically a function form of the macro to convert
    //an (r, c) pair into an index into the board but,
    //since it's a function, there's some nice type checking

    return this->board[__COORD_TO_IDX(this, row, col)];
}

bool uni_board_get_bc(uni_board* this, size_t row, size_t col) {
    //Same as uni_board_get, but with an added bounds check
    //when indexing (which is really useful when enforcing
    //the rules of the game)

    if(row > 0 && row < this->rows && col > 0 && col < this->cols)
        return uni_board_get(this, row, col);
    return false;
}

void uni_board_set(uni_board* this, size_t row, size_t col) {
    //Makes the cell at (row, col) alive (true)

    this->board[__COORD_TO_IDX(this, row, col)] = true;
}

void uni_board_kill(uni_board* this, size_t row, size_t col) {
    //Makes the cell at (row, col) dead (false)

    this->board[__COORD_TO_IDX(this, row, col)] = false;
}

void uni_board_flip(uni_board* this, size_t row, size_t col) {
    //Flips the cell at (row, col), making alive (true) cells dead (false)
    //and dead cells alive

    size_t board_idx = __COORD_TO_IDX(this, row, col);
    this->board[board_idx] = !this->board[board_idx];
}

void uni_board_print(uni_board* this) {
    //Prints the board to stdout

    for(size_t r = 0; r < this->rows; r++) {
        for(size_t c = 0; c < this->cols; c++) {
            const bool alive = this->board[__COORD_TO_IDX(this, r, c)];
            /*if(alive) SetConsoleTextAttribute(
                    cmd_handle, BACKGROUND_INTENSITY |
                    BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN
            );
            else SetConsoleTextAttribute(
                cmd_handle,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN
            );*/

            if(alive) printf("\x1b[47m ");
            else printf("\x1b[40m ");
        }

        printf("\x1b[49m\n");
    }
}

size_t uni_board_alive_neighbors(uni_board* this, size_t row, size_t col) {
    //| Counts the number of alive neighbors of the cell (row, col)
    //|
    //| Arguments:
    //|     row: Row position of cell
    //|     col: Column position of cell
    //|
    //| Returns:
    //|     Number of alive neighbors of the cell (row, col)

    size_t ncount = 0;
    for(int ro = -1; ro < 2; ro++) for(int co = -1; co < 2; co++)
        if(ro != 0 || co != 0)
            ncount += uni_board_get_bc(this, row+ro, col+co);

    return ncount;
}

void uni_board_destroy(uni_board* this) {
    //Destroys an instance of a board. The only thing required
    //other than freeing the instance itself is to free the board
    //array as well. This function simply compresses those two operations

    free(this->board);
    free(this);
}
