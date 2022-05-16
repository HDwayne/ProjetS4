/**
 * @file installos.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for the OS installer
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

/**
 * @brief Format disk
 *
 * @param dirname
 * @param size
 * @param diskid
 * @return int, Success code or error code depending on whether successful or failure
 */
int format(char *dirname, int size, int diskid) {
    char filename[strlen(dirname) + 4];
    snprintf(filename, strlen(dirname) + 4, "%sd%d", dirname, diskid);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FILE_OPEN));
        return ERROR;
    }
    unsigned char zero = 0;
    for (int i = 0; i < size; i++) {
        int nb_write = (int)fwrite(&zero, 1, 1, fp);
        if (nb_write != 1)
            return ERROR;
    }
    if (fclose(fp) == EOF) {
        fprintf(stderr, "%s\n", LangGet(ERROR_FILE_CLOSE));
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief OS installer
 *
 * @param argc
 * @param argv
 * @return int, Success code or error code depending on whether successful or failure
 */
int main(int argc, char **argv){
    /* The above code is initializing the terminal, getting the max size of the terminal, and printing the
    title of the program. */
    initTerm();
    int col;
    int row;
    terminal_get_max_size(&col, &row);
    terminal_cursor((int)((col / 2) - (strlen(OS_TITLE)) / 2), 2);
    fprintf(stdout, "%s\n", OS_TITLE);
    terminal_cursor(0, 4);

    /* The above code is checking to see if the user has entered the correct number of arguments. If the
    user has entered the wrong number of arguments, the program will print out the usage statement and
    exit. */
    if (argc != 2) {
        terminal_print(LangGet(WARNING_INSTALLOS_USAGE), TERMINAL_ORANGE);
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    /* Allocating memory for the list_kid array. */
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
    }
    else {
        fprintf(stdout, "%s\n", LangGet(ERROR_READ_DIR));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    cpt++;
    strcpy(list_kid[cpt], LangGet(INPUT_CANCEL));


    /* The code is asking the user to select the installation disk. */
    terminal_cursor(4, 3);
    fprintf(stdout, "%s", LangGet(OUTPUT_OSINSTALL_SELECT_DISK_NAME));
    terminal_cursor(4, row - 4);
    fprintf(stdout, "%s", LangGet(OUTPUT_SELECT_CHOICE_HELP));
    terminal_cursor((int)((col / 2) - (strlen(LangGet(OUTPUT_CREDITS))) / 2), row - 1);
    fprintf(stdout, "%s", LangGet(OUTPUT_CREDITS));
    setborder();
    int selected = menu(list_kid, cpt);

    /* The above code is printing the title of the program. */
    terminal_clear();
    terminal_cursor((int)((col / 2) - (strlen(OS_TITLE)) / 2), 2);
    fprintf(stdout, "%s\n", OS_TITLE);
    terminal_cursor(0, 4);

    /* The above code is checking if the user has selected the cancel option. If they have, then the
    program will exit. */
    if (selected == cpt) {
        terminal_print(LangGet(OUTPUT_OSINSTALL_CANCEL), TERMINAL_RED);
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return SUCCESS;
    }

    if (init_disk_sos(argv[1], selected) == ERROR) {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    
    /* The above code is asking the user if he wants to format the disk. (if the disk already have the OS installed) */
    if (virtual_disk_sos->super_block.first_free_byte != 0) {
        /* The above code is allocating memory for the yes_no array. */
        char **yes_no = (char **)malloc(3 * sizeof(char *));
        if (yes_no == NULL) {
            fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
            fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
            exitTerm();
            return ERROR;
        }
        for (int i = 0; i < 3; i++) {
            yes_no[i] = (char *)malloc(MAX_FILE_SIZE * sizeof(char));
            if (yes_no[i] == NULL) {
                fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
                fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
                exitTerm();
                return ERROR;
            }
        }

        strcpy(yes_no[0], LangGet(INPUT_YES));
        strcpy(yes_no[1], LangGet(INPUT_NO));
        strcpy(yes_no[2], LangGet(INPUT_CANCEL));

        /* The above code is asking the user if he wants to format the drive. */
        terminal_cursor(4, 3);
        fprintf(stdout, "%s", LangGet(OUTPUT_OSINSTALL_FORMAT_ASK_ALREADY_INSTALLED));
        terminal_cursor(4, row - 4);
        fprintf(stdout, "%s", LangGet(OUTPUT_SELECT_CHOICE_HELP));
        terminal_cursor((int)((col / 2) - (strlen(LangGet(OUTPUT_CREDITS))) / 2), row - 1);
        fprintf(stdout, "%s", LangGet(OUTPUT_CREDITS));
        setborder();
        int choix = menu(yes_no, 2);

        for (int i = 0; i < 3; ++i) {
            free(yes_no[i]);
        }
        free(yes_no);

        terminal_clear();
        terminal_cursor((int)((col / 2) - (strlen(OS_TITLE)) / 2), 2);
        fprintf(stdout, "%s\n", OS_TITLE);
        terminal_cursor(0, 4);

        /* Checking if the user wants to format the disk. If so, it will format the disk. */
        if (choix == 0) {
            if (shutdown_disk_sos() == ERROR) {
                fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
                exitTerm();
                return ERROR;
            }
            if (format(argv[1], 50000, selected) == ERROR) {
                fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
                exitTerm();
                return ERROR;
            }
            if (init_disk_sos(argv[1], selected) == ERROR) {
                fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
                exitTerm();
                return ERROR;
            }
            terminal_print(LangGet(OUTPUT_OSINSTALL_FORAMT_SUCCESS), TERMINAL_GREEN);
        } else {
            terminal_print(LangGet(OUTPUT_OSINSTALL_CANCEL), TERMINAL_RED);
            fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
            exitTerm();
            return ERROR;
        }
    }


    update_first_free_byte();
    session_t user;
    user.userid = ROOT_UID;

    
    /* Allocating memory for the variables username, password, verifpwd and password_hash. */
    char *username = malloc(sizeof(char) * FILENAME_MAX_SIZE);
    if (username == NULL) {
        fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    char *password = malloc(sizeof(char) * FILENAME_MAX_SIZE);
    if (password == NULL) {
        fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    char *verifpwd = malloc(sizeof(char) * FILENAME_MAX_SIZE);
    if (verifpwd == NULL) {
        fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    char *password_hash = malloc(sizeof(char) * (SHA256_BLOCK_SIZE * 2 + 1));
    if (password_hash == NULL) {
        fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    /* Asking the user for a username. */
    fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_SUPERUSER_NAME));

    terminal_canonique();
    if (read_cmd(username, FILENAME_MAX_SIZE) == ERROR) {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    terminal_non_canonique();

    if (!strcmp(username, "")) {
        strcpy(username, "root");
        fprintf(stdout, "%s\n", username);
    }

    /* The above code is asking the user to enter the password twice while the password is not valid. */
    do {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_SUPERUSER_PWD));
        if (read_cmd(password, FILENAME_MAX_SIZE) == ERROR) {
            fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
            exitTerm();
            return ERROR;
        }
        fprintf(stdout, "%s\n", LangGet(OUTPUT_OSINSTALL_SUPERUSER_PWD2));
        if (read_cmd(verifpwd, FILENAME_MAX_SIZE) == ERROR) {
            fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
            exitTerm();
            return ERROR;
        }
    } while (!is_valid_password(password, verifpwd));

    if (init_user(username, password) == ERROR) {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    sha256ofString((BYTE *)password, password_hash);

    /* The above code is creating a file called passwd in the directory /d<selected> and writing the
    username and password hash in it. */
    file_t file;
    char *chaine = (char *)malloc((strlen(password_hash) + strlen(username) + 1) * sizeof(char));
    if (chaine == NULL) {
        fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    strcpy(chaine, username);
    strcat(chaine, " ");
    strcat(chaine, password_hash);
    strcat(chaine, "\n");
    memcpy(file.data, chaine, strlen(chaine));
    
    file.size = strlen(chaine);

    char *namefile = (char *)malloc((strlen("passwd") + 5) * sizeof(char));
    if (namefile == NULL) {
        fprintf(stdout, "%s\n", LangGet(ERROR_MALLOC));
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }
    snprintf(namefile, strlen("passwd") + 5, "%s_d%d", "passwd", selected);
    if (write_file(namefile, file, user) == ERROR) {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    /* Shutdown OS and cleaning up the memory allocated for the variables. */
    if (shutdown_disk_sos() == ERROR) {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
        exitTerm();
        return ERROR;
    }

    free(chaine);
    free(namefile);
    for (int i = 0; i < MAX_DISK_KID + 1; ++i) {
        free(list_kid[i]);
    }
    free(list_kid);
    free(username);
    free(verifpwd);
    free(password);
    free(password_hash);

    terminal_print(LangGet(OUTPUT_OSINSTALL_SUCCESS), TERMINAL_GREEN);
    fprintf(stdout, "%s\n", LangGet(OUTPUT_PRESS_EXIT));
    exitTerm();
    return SUCCESS;
}