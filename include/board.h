#ifndef UNI_BOARD_H_INCLUDED_
#define UNI_BOARD_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

//Converts a row(r) and column(c) into an index for the board array
#define __COORD_TO_IDX(board, r, c) r*board->cols + c 

typedef struct uni_board_t {
    bool* board;
    size_t rows, cols;
    size_t size;
} uni_board;

uni_board* uni_board_init(size_t, size_t, bool*);

void uni_board_randomize(uni_board*);

bool uni_board_get(uni_board*, size_t, size_t);
bool uni_board_get_bc(uni_board*, size_t, size_t);
void uni_board_set(uni_board*, size_t, size_t);
void uni_board_kill(uni_board*, size_t, size_t);
void uni_board_flip(uni_board*, size_t, size_t);

void uni_board_print(uni_board*);

size_t uni_board_alive_neighbors(uni_board*, size_t, size_t);

void uni_board_destroy(uni_board*);

#endif  //UNI_BOARD_H_INCLUDED_
