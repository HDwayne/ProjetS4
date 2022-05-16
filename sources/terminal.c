#include "../headers/terminal.h"
#include <stdio.h>

int terminal_canonique() {
	struct termios term;
	/* retour en mode ligne */
	tcgetattr(fileno(stdin), &term);				  /* lit les flags du terminal dans term */
	term.c_lflag |= ICANON;							  /* mode canonique */
	term.c_lflag |= ECHO;							  /* rétablit l'écho */
	if (tcsetattr(fileno(stdin), TCSANOW, &term) < 0) { /* écrit les flags depuis term */
		perror("Term_canonique: problème d'initialisation ");
		return 0;
	}
	return 1;
}

int terminal_non_canonique() {
	struct termios term;
	tcgetattr(fileno(stdin), &term);				  /* lit les flags du terminal dans term */
	term.c_lflag &= ~ICANON;						  /* mode non-canonique */
	term.c_lflag &= ~ECHO;							  /* supprime l'écho */
	term.c_cc[VMIN] = 1;							  /* nombre min de caractères */
	term.c_cc[VTIME] = 1;							  /* latence (timeout) 1/10e de seconde (0: pas de latence) */
	if (tcsetattr(fileno(stdin), TCSANOW, &term) < 0) { /* écrit les flags depuis term */
		perror("Term_non_canonique: problème d'initialisation ");
		return 0;
	}
	return 1;
}

void terminal_get_max_size(int *cols, int *rows) {
	struct winsize size;
	ioctl(1, TIOCGWINSZ, &size);
	*cols = size.ws_col;
	*rows = size.ws_row;
}

void menu_selected(int pos, char **list_item, int max)
{
	int col;
	int row;
	terminal_get_max_size(&col, &row);
	for (int i = 0; i <= max; i++) {
		// terminal_cursor(0,0);
		// fprintf(stdout, "%d %d", pos, i);

		terminal_cursor(5, 5 + i);
		if (pos == i) {
			if (!strcmp(list_item[i], "cancel"))
				terminal_print(list_item[i], TERMINAL_RED);
			else
				terminal_print(list_item[i], TERMINAL_BLUE);
		}
		else
			fprintf(stdout, "%s\n", list_item[i]);
	}
}

int menu(char **liste_elem, int max) {
	int pos = 0;
	char lettre;
	menu_selected(pos, liste_elem, max);
	do {
		read(STDIN_FILENO, &lettre, sizeof(char));
		// terminal_cursor(2,2);
		// fprintf(stdout, "%c", lettre);
		// fflush(stdout);

		if (lettre == ' ')
			return max;
		else if (lettre == 'A' && pos > 0){
			pos--;
			menu_selected(pos, liste_elem, max);
		} else if (lettre == 'B' && pos < max) {
			pos++;
			menu_selected(pos, liste_elem, max);
		}
	} while (lettre != TC_KEY_ENTER);
	return pos;
}


void setborder() {
	int col;
	int row;
	terminal_get_max_size(&col, &row);
	for (int i = 4; i < col - 1; i++){
		terminal_cursor(i, 4);
		if (i == 4)
			fprintf(stdout, "┌");
		else if (i == col - 2)
			fprintf(stdout, "┐");
		else
			fprintf(stdout, "─");
	}

	for (int i = 5; i < row - 5; i++){
		terminal_cursor(4, i);
		fprintf(stdout, "│");
		terminal_cursor(col - 2, i);
		fprintf(stdout, "│");
	}

	for (int i = 4; i < col - 1; i++){
		terminal_cursor(i, row - 5);
		if (i == 4)
			fprintf(stdout, "└");
		else if (i == col - 2)
			fprintf(stdout, "┘");
		else
			fprintf(stdout, "─");
	}
}

/**
 * @brief init terminal
 */
void initTerm() {
	terminal_clear();
	terminal_non_canonique();
	printf("\e[?25l");
}

/**
 * @brief reset terminal
 */
void exitTerm() {
	char lettre;
	read(STDIN_FILENO, &lettre, sizeof(char));
	printf("\e[?25h");
	terminal_cursor(0, 0);
	terminal_canonique();
	terminal_clear();
}

void terminal_set_editor_mode(char* filename, int line_to_edit, int nb_line_tt){
    terminal_clear();
	int col;
    int row;
    terminal_get_max_size(&col, &row);

    terminal_cursor(0,0);
    fprintf(stdout, "┌──");
    for (int i = 4; i < col-2; i++){
		terminal_cursor(i, 0);
        terminal_print_bg(" ", TERMINAL_BLACK, TERMINAL_BG_WHT);
	}
    terminal_cursor(col-2,0);
    fprintf(stdout, "──┐");

    terminal_cursor(5,0);
    terminal_print_bg(filename, TERMINAL_BLACK, TERMINAL_BG_WHT);

    terminal_cursor((int)((col / 2) - (strlen("Text editor")) / 2), 0);
    terminal_print_bg("Text editor", TERMINAL_BLACK, TERMINAL_BG_WHT);

	char pos[10];
	char end[10];
	sprintf(pos, "%d", line_to_edit);
	sprintf(end, "%d", nb_line_tt);
	terminal_cursor((int)(col-(strlen(pos)+1+strlen(end))), 0);
    terminal_print_bg(pos, TERMINAL_BLACK, TERMINAL_BG_WHT);
	terminal_cursor((int)(col-(1+strlen(end))), 0);
    terminal_print_bg("/", TERMINAL_BLACK, TERMINAL_BG_WHT);
	terminal_cursor((int)(col-(strlen(end))), 0);
    terminal_print_bg(end, TERMINAL_BLACK, TERMINAL_BG_WHT);
    
    for (int i = 2; i < row; i++){
		terminal_cursor(0, i);
		fprintf(stdout, "│");
		terminal_cursor(col, i);
		fprintf(stdout, "│");
	}

    terminal_cursor(0, row);
    fprintf(stdout, "└─ n - insert ──── b - duplicate ─");
	for (int i =35; i < col; i++){
		terminal_cursor(i, row);
        fprintf(stdout, "─");
	}
    terminal_cursor(col, row);
    fprintf(stdout, "┘");
}

int get_nb_cara(size_t msg_size, char (*text)[msg_size]){
    int nb_cara = 0;
    for (int i = 0; text[i] != NULL; i++){
        if (strlen(text[i]) != 0)
            nb_cara += strlen(text[i]);
        else
            nb_cara += 1;
    }
    return nb_cara;
}

void terminal_editor_elem(size_t array_size, size_t msg_size, char (*text)[msg_size], int pos, int offset, bool toEdit){
	fflush(stdout);

	int edit_offset=0;
	int col;
    int row;
    terminal_get_max_size(&col, &row);

	for (int i = 0; i+2<row-1 && i+offset<array_size; i++){
		terminal_cursor(1, i+2+edit_offset);
		if (i==pos){
			fprintf(stdout, "%s┝ %s%s", TERMINAL_GREEN, text[i+offset], TERMINAL_RESET);
			if (toEdit){
				terminal_cursor(1,i+3);
				fprintf(stdout, "│ new text : ");
				edit_offset=1;
			}
		} else
			fprintf(stdout, "┝ %s", text[i+offset]);
	}

	int cara = get_nb_cara(msg_size, text);
	char cara_str[10];
	sprintf(cara_str, "%d", cara);
	
	terminal_cursor((int)(col-(strlen(cara_str))), row);
	if (cara < MAX_FILE_SIZE)
		fprintf(stdout, "%s%s%s%s", TERMINAL_BLACK, TERMINAL_BG_WHT, cara_str, TERMINAL_RESET);
	else
		fprintf(stdout, "%s%s%s%s", TERMINAL_WHITE, TERMINAL_BG_RED, cara_str, TERMINAL_RESET);
	fflush(stdout);
}