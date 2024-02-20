#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#include "board.h"

enum CMD_ARGS {
    ARG_RANDOM_MODE,
    ARG_SET_MODE,
    ARG_COUNT
};

void update_board_state(uni_board*);
unsigned char parse_cmd_args(int, char**);

bool clear_console(HANDLE, CONSOLE_SCREEN_BUFFER_INFO);

int main(int argc, char** argv) {
    //Set a random seed for board generation
    srand(time(NULL));

    //Parse command-line arguments
    const unsigned char cmd_args = parse_cmd_args(argc, argv);

    //Get console handle
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if(hconsole == INVALID_HANDLE_VALUE) {
        fputs("[ERROR] Console handle aquisition error...\n", stderr);
        return -1;
    }

    //Save the console attributes for restoration at the end
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if(!GetConsoleScreenBufferInfo(hconsole, &console_info)) {
        fputs("[ERROR] Failed to get console buffer info...\n", stderr);
        return -1;
    }
    WORD saved_attrs = console_info.wAttributes;

    //Create board
    bool board_init_error = false;
    uni_board* board = uni_board_init(15, 50, &board_init_error);
    if(board_init_error) {
        fputs("[ERROR] Board initialization error...\n", stderr);
        return -1;
    }

    uni_board_randomize(board);

    bool running = true;
    while(running) {
        if(!clear_console(hconsole, console_info)) running = false;
        uni_board_print(board);

        const char cmd_input[25];
        fgets(cmd_input, sizeof(cmd_input), stdin);
        switch(cmd_input[0]) {
            case 'q': {
                running = false;
            } break;
        }

        update_board_state(board);
    }

    puts("Shutting down...\n");
    
    //Cleanup
    uni_board_destroy(board);
    SetConsoleTextAttribute(hconsole, saved_attrs);
    return 0;
}

void update_board_state(uni_board* board) {
    //| Updates the state of the board based upon the rules of Conway's
    //| Game of Life:
    //|  * Any live cell with fewer than two neighbors dies
    //|  * Any live cell with more than three neighbors dies
    //|  * Any live cell with exactly two of three live neighbors lives on
    //|  * Any dead cell with exactly three neighbors becomes alive 

    for(size_t r = 0; r < board->rows; r++) {
        for(size_t c = 0; c < board->cols; c++) {
            const size_t ncount = uni_board_alive_neighbors(board, r, c);
            if(uni_board_get(board, r, c)) {
                if(ncount < 2 || ncount > 3) uni_board_kill(board, r, c);
            } else if(ncount == 3) uni_board_set(board, r, c);
        }
    }
}

unsigned char parse_cmd_args(int argc, char** argv) {
    for(size_t i = 1; i < argc; i++) {
        
    }
}

bool clear_console(HANDLE hconsole, CONSOLE_SCREEN_BUFFER_INFO console_info) {
    DWORD console_size = console_info.dwSize.X * console_info.dwSize.Y;
    DWORD written;

    if(!FillConsoleOutputCharacter(
        hconsole, ' ', console_size, (COORD){0, 0}, &written
    )) {
        fputs("[ERROR] Failed to fill console output character", stderr);
        return false;
    }

    SetConsoleCursorPosition(hconsole, (COORD){0, 0});

    return true;
}
