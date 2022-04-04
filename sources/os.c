/**
 * @file os.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief main file of the OS
 * @version 0.1
 * @date 2022-02-14
 */

#include "../headers/os_defines.h"
#include "../headers/language.h"
#include "../headers/layer1.h"
#include "../headers/layer2.h"
#include "../headers/layer3.h"
#include "../headers/layer4.h"
#include "../headers/layer5.h"
#include "../headers/terminal.h"
#include <dirent.h>

int main(int argc, char **argv) {
    /* initialize the terminal. */
    initTerm();
    int col;
    int row;
    terminal_get_max_size(&col, &row);
    terminal_cursor((int)((col / 2) - (strlen(OS_TITLE)) / 2), 2);
    fprintf(stdout, "%s\n", OS_TITLE);
    terminal_cursor(0, 4);

    /* This is the first check of the program. If the user doesn't give the right number of arguments,
    the program will print the usage and exit. */
    if (argc != 2) {
        terminal_print(LangGet(WARNING_OS_USAGE), TERMINAL_ORANGE);
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    /* It's a memory allocation of list_kid. */
    char **list_kid = (char **)malloc((MAX_DISK_KID + 1) * sizeof(char *));
    if (list_kid == NULL) {
        fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    for (int i = 0; i < (MAX_DISK_KID + 1); i++) {
        list_kid[i] = (char *)malloc(MAX_FILE_SIZE * sizeof(char));
        if (list_kid[i] == NULL) {
            fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
            fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
            exitTerm();
            return ERROR;
        }
    }

    /* The above code is reading the directory and storing the names of the files in the list_kid array. */
    struct dirent *dir;
    DIR *d = opendir(argv[1]);
    int cpt = -1;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(".", dir->d_name) && strcmp("..", dir->d_name) && strcmp("DO_NOT_DELETE", dir->d_name)) {
                cpt++;
                strcpy(list_kid[cpt], dir->d_name);
            }
        }
        if (closedir(d) == -1) {
            fprintf(stdout, "%s\n", LangGet(ERROR_CLOSE_DIR));
            fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
            exitTerm();
            return ERROR;
        }
    } else {
        fprintf(stdout, "%s\n", LangGet(ERROR_READ_DIR));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    cpt++;
    strcpy(list_kid[cpt], LangGet(INPUT_CANCEL));

    /* The code is asking the user to select the disk. */
    terminal_cursor(4, 3);
    fprintf(stdout, "%s\n", LangGet(OUTPUT_OS_SELECT_DISK));
    terminal_cursor(4, row - 4);
    fprintf(stdout, "%s\n", LangGet(OUTPUT_SELECT_CHOICE_HELP));
    terminal_cursor((int)((col / 2) - (strlen(LangGet(OUTPUT_CREDITS))) / 2), row - 1);
    fprintf(stdout, "%s\n", LangGet(OUTPUT_CREDITS));
    setborder();
    int selected = menu(list_kid, cpt);

    terminal_clear();
    terminal_cursor((int)((col / 2) - (strlen(OS_TITLE)) / 2), 2);
    fprintf(stdout, "%s\n", OS_TITLE);
    terminal_cursor(0, 4);

    /* If the user select the `cancel` option, the program will exit. */
    if (selected == cpt) {
        terminal_print(LangGet(OUTPUT_OS_SHUTDOWN), TERMINAL_GREEN);
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return SUCCESS;
    }

    printf("\e[?25h");
    terminal_canonique();

    if (init_disk_sos(argv[1], selected) == ERROR) {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    if (virtual_disk_sos->super_block.first_free_byte == 0) {
        terminal_print(LangGet(ERROR_OS_NOT_FOUND), TERMINAL_RED);
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    fprintf(stdout, "%s %d\n", LangGet(OUTPUT_OS_STOP_SHELL), terminal_shell());
    terminal_print(LangGet(OUTPUT_OS_SHUTDOWN), TERMINAL_GREEN);
    fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
    exitTerm();
    return shutdown_disk_sos();
}