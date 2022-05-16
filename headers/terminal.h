#ifndef TERMINAL_OS_DEFINES_H
#define TERMINAL_OS_DEFINES_H

#include <stdio.h>
#include <sys/termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "os_defines.h"

#define TERMINAL_RESET  "\x1B[0m"
#define TERMINAL_RED  "\x1B[1;31m"
#define TERMINAL_GREEN  "\x1B[1;32m"
#define TERMINAL_YELLOW  "\x1B[1;33m"
#define TERMINAL_BLUE  "\x1B[1;34m"
#define TERMINAL_MAGENTA  "\x1B[1;35m"
#define TERMINAL_CYAN  "\x1B[1;36m"
#define TERMINAL_WHITE  "\x1B[1;37m"
#define TERMINAL_ORANGE "\x1B[38;5;166m"
#define TERMINAL_BLACK "\x1B[38;5;16m"
#define TERMINAL_BG_NRM "\x1B[40m"
#define TERMINAL_BG_RED "\x1B[41m"
#define TERMINAL_BG_GRN "\x1B[42m"
#define TERMINAL_BG_YEL "\x1B[43m"
#define TERMINAL_BG_BLU "\x1B[44m"
#define TERMINAL_BG_MAG "\x1B[45m"
#define TERMINAL_BG_CYN "\x1B[46m"
#define TERMINAL_BG_WHT "\x1B[47m"

#define terminal_clear() printf("\x1B[2J")
#define terminal_clear_line() puts("\33[2K\r")
#define terminal_cursor(X, Y) printf("\033[%d;%dH", Y, X)
#define terminal_print_bg(TEXT, COLOR, BG) fprintf(stdout, "%s%s%s%s\n", COLOR, BG, TEXT, TERMINAL_RESET)
#define terminal_print(TEXT, COLOR) fprintf(stdout, "%s%s%s\n", COLOR, TEXT, TERMINAL_RESET)

#define TC_KEY_ESCAPE 0x1B
// #define TC_KEY_F1 0x1b4f50
// #define TC_KEY_F1_TTY 0x1b5b5b41
// #define TC_KEY_F2 0x1b4f51
// #define TC_KEY_F2_TTY 0x1b5b5b42
// #define TC_KEY_F3 0x1b4f52
// #define TC_KEY_F3_TTY 0x1b5b5b43
// #define TC_KEY_F4 0x1b4f53
// #define TC_KEY_F4_TTY 0x1b5b5b44
// #define TC_KEY_F5 0x1b5b31357e
// #define TC_KEY_F5_TTY 0x1b5b5b45
// #define TC_KEY_F6 0x1b5b31377e
// #define TC_KEY_F7 0x1b5b31387e
// #define TC_KEY_F8 0x1b5b31397e
// #define TC_KEY_F9 0x1b5b32307e
// #define TC_KEY_F10 0x1b5b32317e
// #define TC_KEY_F11 0x1b5b32337e
// #define TC_KEY_F12 0x1b5b32347e
// #define TC_KEY_ARROW_UP 0x1b5b41
// #define TC_KEY_ARROW_DOWN 0x1b5b42
// #define TC_KEY_ARROW_LEFT 0x1b5b44
// #define TC_KEY_ARROW_RIGHT 0x1b5b43
// #define TC_KEY_TAB 0x9
// #define TC_KEY_RETURN 0xa
#define TC_KEY_ENTER 0xa
// #define TC_KEY_INSERT 0x1b5b327e
// #define TC_KEY_HOME 0x1b5b48
// #define TC_KEY_PAGE_UP 0x1b5b357e
// #define TC_KEY_DELETE 0x1b5b337e
// #define TC_KEY_END 0x1b5b46
// #define TC_KEY_PAGE_DOWN 0x1b5b367e
// #define TC_KEY_EURO 0xffffffffffe181ac
// #define TC_KEY_GBP 0xffffffffffffc1a3

void terminal_get_max_size(int *cols, int *rows);
int terminal_non_canonique();
int terminal_canonique();
int menu(char** liste_elem, int max);
void menu_selected(int pos, char** list_item, int max);
void setborder();
void initTerm();
void exitTerm();
void terminal_set_editor_mode(char* filename, int line_to_edit, int nb_line_tt);
int get_nb_cara(char* text[(size_t)MAX_MSG]);
void terminal_editor_elem(size_t array_size, size_t msg_size, char (*text)[msg_size], int pos, int offset, bool toEdit);
#endif // TERMINAL_OS_DEFINES_H