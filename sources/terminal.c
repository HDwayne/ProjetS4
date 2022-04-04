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