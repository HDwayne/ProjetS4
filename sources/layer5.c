/**
 * @file layer5.c
 * @author  HERZBERG Dwayne and BERLIN Florian
 * @brief Source code for layer5 of the ScratchOs : shell and commands
 * @version 0.1
 * @date 2022-02-14
 */

#include "../headers/layer5.h"

/**
 * @brief Read user input
 *
 * @param cmd
 * @param size
 * @return int, Success code or error code depending on whether successful or failure
 */
int read_cmd(char *cmd, int size) {
    fgets(cmd, size, stdin);
    if (ferror(stdin)) {
        terminal_print(LangGet(ERROR_USER_INPUT), TERMINAL_RED);
        // fprintf(stderr, "%s\n", LangGet(ERROR_USER_INPUT));
        return ERROR;
    }
    int got_size = (int)strlen(cmd);
    if (got_size == size)
        while (getchar() != '\n');
    else
        cmd[size - 1] = '\0';
    cmd[strcspn(cmd, "\n")] = 0;
    return SUCCESS;
}

/**
 * @brief Diplay to conole all infos of one inode
 *
 * @param inode
 * @param user
 */
void display_ls_l(inode_t inode, session_t user) {
    char uright[3];
    if (inode.uid == user.userid) {
        if (inode.uright == rw)
            strcpy(uright, "--");
        else if (inode.uright == rW)
            strcpy(uright, "-w");
        else if (inode.uright == Rw)
            strcpy(uright, "r-");
        else if (inode.uright == RW)
            strcpy(uright, "rw");
    } else {
        if (inode.oright == rw)
            strcpy(uright, "--");
        else if (inode.oright == rW)
            strcpy(uright, "-w");
        else if (inode.oright == Rw)
            strcpy(uright, "r-");
        else if (inode.oright == RW)
            strcpy(uright, "rw");
    }
    fprintf(stdout, "%s %d %s %d %s %s\n", uright, inode.uid, virtual_disk_sos->users_table[inode.uid].login, inode.size, inode.mtimestamp, inode.filename);
}

/**
 * @brief Executes ls command
 * @param args
 * @param session
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_ls(cmd_t args, session_t user) {
    int last_inode = get_unused_inode(virtual_disk_sos->inodes);
    if (args.nbArgs > 2) {
        terminal_print(LangGet(ERROR_COMMAND_LS_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_LS_USAGE));
        return ERROR;
    }
    else if (args.nbArgs == 2) {
        if (strcmp(args.tabArgs[1], "-l") != 0) {
            terminal_print(LangGet(ERROR_COMMAND_ARGS_UNKNOWN), TERMINAL_RED);
            terminal_print(LangGet(ERROR_COMMAND_LS_USAGE), TERMINAL_RED);
            // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ARGS_UNKNOWN));
            // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_LS_USAGE));
            return ERROR;
        } else {
            for (int i = 0; i < last_inode; i++) {
                inode_t inode = virtual_disk_sos->inodes[i];
                display_ls_l(inode, user);
            }
        }
    } else {
        for (int i = 0; i < virtual_disk_sos->super_block.number_of_files; i++)
            fprintf(stdout, "%s ", virtual_disk_sos->inodes[i].filename);
        fprintf(stdout, "\n");
    }
    return SUCCESS;
}

/**
 * @brief Executes cat command
 * @param args
 * @param session
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_cat(cmd_t args, session_t user) {
    if (args.nbArgs != 2) {
        terminal_print(LangGet(ERROR_COMMAND_CAT_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_CAT_USAGE));
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if (index_inode == INODE_TABLE_SIZE) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1], TERMINAL_RESET);
        return ERROR;
    }
    if (!has_rights(index_inode, user.userid, Rw)) {
        terminal_print(LangGet(ERROR_FILE_RIGHTS), TERMINAL_RED);
        // fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS));
        return ERROR;
    }
    file_t file;
    if (read_file(args.tabArgs[1], &file) == ERROR)
        return ERROR;
    fprintf(stdout, "%s\n", file.data);
    return SUCCESS;
}

/**
 * @brief Executes rm command
 * @param args
 * @param session
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_rm(cmd_t args, session_t user) {
    if (args.nbArgs != 2) {
        terminal_print(LangGet(ERROR_COMMAND_RM_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_RM_USAGE));
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if (index_inode == INODE_TABLE_SIZE) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1], TERMINAL_RESET);
        return ERROR;
    }
    if (!has_rights(index_inode, user.userid, rW)) {
        terminal_print(LangGet(ERROR_FILE_RIGHTS), TERMINAL_RED);
        // fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS));
        return ERROR;
    }
    if (delete_file(args.tabArgs[1]) == ERROR)
        return ERROR;
    fprintf(stderr, "%s%s %s%s\n", TERMINAL_GREEN, args.tabArgs[1], LangGet(OUTPUT_COMMAND_RM), TERMINAL_RESET);
    return SUCCESS;
}

/**
 * @brief Executes cr command
 *
 * @param args
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_cr(cmd_t args, session_t user) {
    if (args.nbArgs != 2) {
        terminal_print(LangGet(ERROR_COMMAND_CR_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_CR_USAGE));
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if (index_inode != INODE_TABLE_SIZE) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]);
        return ERROR;
    }
    file_t file;
    file.size = 1;
    file.data[0] = '\0';
    if (write_file(args.tabArgs[1], file, user) == ERROR)
        return ERROR;
    fprintf(stdout, "%s%s %s%s\n", TERMINAL_GREEN, args.tabArgs[1], LangGet(OUTPUT_COMMAND_CR), TERMINAL_RESET);
    return SUCCESS;
}

/**
 * @brief Executes edit command
 *
 * @param args
 * @param user
 * @return int
 */
int cmd_edit(cmd_t args, session_t user) {
    if (args.nbArgs != 2) {
        terminal_print(LangGet(ERROR_COMMAND_EDIT_USAGE), TERMINAL_ORANGE);
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if (index_inode == INODE_TABLE_SIZE) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1], TERMINAL_RESET);
        return ERROR;
    }
    if (!has_rights(index_inode, user.userid, rW)) {
        terminal_print(LangGet(ERROR_FILE_RIGHTS), TERMINAL_RED);
        return ERROR;
    }


    file_t file;
    if (read_file(args.tabArgs[1], &file) == ERROR)
        return ERROR;

    //fprintf(stdout, "%s\n", file.data);

    int nb_line_tt = 1;
    for (uint i = 0; file.data[i] != '\0'; i++)
        if (file.data[i] == '\n')
            nb_line_tt++;

    char text[1000][MAX_MSG];

    // TODO
    int i = 0;
    const char delim[1] = "\n";
    char *token;
    token = strtok((char*)file.data, delim);
    while( token != NULL && strlen(token) != 0 ) {
        //fprintf(stdout, "%s\n", token);
        strcpy(text[i], token);
        strcat(text[i], "\n");
        i++;
        token = strtok(NULL, delim);
    }

    terminal_clear();
    terminal_non_canonique();
    printf("\e[?25l");

    char lettre;
    int pos=0;
    int offset=0;
    int cara=0;

    int col;
    int row;
    terminal_get_max_size(&col, &row);

    do {
        terminal_set_editor_mode(args.tabArgs[1], pos+offset+1, nb_line_tt);
        terminal_editor_elem(nb_line_tt, MAX_MSG, text, pos, offset, false);
        read(STDIN_FILENO, &lettre, sizeof(char));
        if (lettre == 'A' && pos > 0){ // up
            if (offset)
                offset--;
            else
                pos--;
            terminal_editor_elem(nb_line_tt, MAX_MSG, text, pos, offset, false);
        } else if (lettre == 'B' && pos < nb_line_tt-1) { // down
            if (pos < row-4)
                pos++;
            else if (offset+pos < nb_line_tt-1)
                offset++;
            terminal_editor_elem(nb_line_tt, MAX_MSG, text, pos, offset, false);
        } else if (lettre == 'C') { // edit - TODO offset edit overwiting
            terminal_set_editor_mode(args.tabArgs[1], pos+offset+1, nb_line_tt);
            terminal_editor_elem(nb_line_tt, MAX_MSG, text, pos, offset, true);
            terminal_cursor(13,pos+3);
            printf("\e[?25h");
	        terminal_canonique();
            fgets(text[pos+offset], MAX_MSG, stdin);
            // text[pos+offset][strcspn(text[pos+offset], "\n")] = 0;
            terminal_non_canonique();
            printf("\e[?25l");
        } else if (lettre == 'D') { // delete
            terminal_cursor(1,pos+2);
            fprintf(stdout, "%s┝ %s%s", TERMINAL_RED, text[pos+offset], TERMINAL_RESET);
            fflush(stdout);
            do {
                read(STDIN_FILENO, &lettre, sizeof(char));
            } while (lettre != 'D' && lettre != 'C');
            if (lettre == 'D') {
                for (int i = pos+offset; i<nb_line_tt; i++)
                    strcpy(text[i], text[i+1]);
                strcpy(text[nb_line_tt], " ");
                if (offset)
                    offset--;
                else if (pos)
                    pos--;
                if (nb_line_tt > 1)
                    nb_line_tt--;
            } else {
                fprintf(stdout, "┝ %s", text[pos+offset]);
                fflush(stdout);
            }
        } else if (lettre == 'b' || lettre == 'n') { // b pour dupliquer, n pour nouvelle ligne
            for (int i = nb_line_tt-1; i>=pos+offset; i--)
                strcpy(text[i+1], text[i]);
            if (lettre == 'n')
                strcpy(text[pos+offset], " ");
            if (offset || pos+1 >= row-3)
                offset++;
            else
                pos++;
            nb_line_tt++;
            terminal_editor_elem(nb_line_tt, MAX_MSG, text, pos, offset, false);
        }
        cara=0;
        for (int i = 0; i < nb_line_tt; i++)
            cara+=strlen(text[i]);
    } while (lettre != TC_KEY_ENTER && cara < MAX_FILE_SIZE);
    
    printf("\e[?25h");
	terminal_cursor(0, 0);
	terminal_canonique();
	terminal_clear();

    fprintf(stdout, "--- text ---\n");
    for (int i = 0; i < nb_line_tt; i++)
        fprintf(stdout, "%s", text[i]);
    fprintf(stdout, "--- ---- ---\n");

    file_t newfile;
    newfile.size = 0;
    memcpy(newfile.data, "", strlen(""));
    
    // fprintf(stdout, "nb: %d\n", nb_line_tt);

    for (int i = 0; i < nb_line_tt; i++) {
        if (strcmp(text[i], "\n") != 0) {
            memcpy(newfile.data + newfile.size, text[i], strlen(text[i]));    
            newfile.size += strlen(text[i]);
            if (i < nb_line_tt-1 && text[i][strlen(text[i])-1] != '\n'){
                memcpy(newfile.data + newfile.size, "\n", strlen("\n"));
                newfile.size += strlen("\n");
            }
        }
    }

    newfile.data[newfile.size-1] = '\0';
    // newfile.size += strlen("\0");
    // newfile.size = strlen((char *)newfile.data);
    newfile.size = cara;
    //fprintf(stdout, "size: %d\n", newfile.size);

    int nb_n=0;
    for (uint i = 0; newfile.data[i] != '\0'; i++)
        if (newfile.data[i] == '\n')
            nb_n++;

   // fprintf(stdout, "nb_n: %d\n", nb_n);

    //fprintf(stdout, "--- data ---\n%s\n--- ---- ---\n", (char*)newfile.data);

    if (write_file(args.tabArgs[1], newfile, user) == ERROR)
        return ERROR;

    return SUCCESS;
}


/**
 * @brief Executes load command
 *
 * @param args
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_load(cmd_t args, session_t user) {
    if (args.nbArgs != 2) {
        terminal_print(LangGet(ERROR_COMMAND_LOAD_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_LOAD_USAGE));
        return ERROR;
    }
    if (load_file_from_host(args.tabArgs[1], user) == ERROR) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_LOAD_LOADING_FILE), args.tabArgs[1], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_LOAD_LOADING_FILE), args.tabArgs[1]);
        return ERROR;
    }
    fprintf(stdout, "%s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_LOAD));
    return SUCCESS;
}

/**
 * @brief Executes store command
 *
 * @param args
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_store(cmd_t args, session_t user) {
    if (args.nbArgs != 2) {
        terminal_print(LangGet(ERROR_COMMAND_STORE_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_STORE_USAGE));
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if (index_inode == INODE_TABLE_SIZE) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]);
        return ERROR;
    }
    if (!has_rights(index_inode, user.userid, RW)) {
        terminal_print(LangGet(ERROR_FILE_RIGHTS), TERMINAL_RED);
        // fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS));
        return ERROR;
    }
    if (store_file_to_host(args.tabArgs[1]) == ERROR) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_STORE_STORING_FILE), args.tabArgs[1], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_STORE_STORING_FILE), args.tabArgs[1]);
        return ERROR;
    }
    fprintf(stdout, "%s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_STORE));
    return SUCCESS;
}

/**
 * @brief Executes chown command
 *
 * @param args
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_chown(cmd_t args, session_t user) {
    if (args.nbArgs != 3) {
        terminal_print(LangGet(ERROR_COMMAND_CHOWN_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_CHOWN_USAGE));
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if (index_inode == INODE_TABLE_SIZE) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]);
        return ERROR;
    }
    int index_login = is_login_in_users_table(args.tabArgs[2]);
    if (index_login == NB_USERS) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_LOGIN_EXIST), args.tabArgs[2], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_LOGIN_EXIST), args.tabArgs[2]);
        return ERROR;
    }
    if (!has_rights(index_inode, user.userid, RW)) {
        terminal_print(LangGet(ERROR_FILE_RIGHTS), TERMINAL_RED);
        // fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS));
        return ERROR;
    }
    uint last_uid = virtual_disk_sos->inodes[index_inode].uid;
    virtual_disk_sos->inodes[index_inode].uid = index_login;
    fprintf(stderr, "%s: %s (%s -> %s)\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_CHOWN), virtual_disk_sos->users_table[last_uid].login, args.tabArgs[2]);
    return SUCCESS;
}

/**
 * @brief Executes chmod command
 *
 * @param args
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_chmod(cmd_t args, session_t user) {
    if (args.nbArgs != 3) {
        terminal_print(LangGet(ERROR_COMMAND_CHMOD_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_CHMOD_USAGE));
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if (index_inode == INODE_TABLE_SIZE) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]);
        return ERROR;
    }
    if (!has_rights(index_inode, user.userid, RW)) {
        terminal_print(LangGet(ERROR_FILE_RIGHTS), TERMINAL_RED);
        // fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS));
        return ERROR;
    }
    if (strcmp(args.tabArgs[2], "rw") == 0 || strcmp(args.tabArgs[2], "0") == 0)
        virtual_disk_sos->inodes[index_inode].oright = rw;
    else if (strcmp(args.tabArgs[2], "rW") == 0 || strcmp(args.tabArgs[2], "1") == 0)
        virtual_disk_sos->inodes[index_inode].oright = rW;
    else if (strcmp(args.tabArgs[2], "Rw") == 0 || strcmp(args.tabArgs[2], "2") == 0)
        virtual_disk_sos->inodes[index_inode].oright = Rw;
    else if (strcmp(args.tabArgs[2], "RW") == 0 || strcmp(args.tabArgs[2], "3") == 0)
        virtual_disk_sos->inodes[index_inode].oright = RW;
    else {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_RIGHT_EXIST), args.tabArgs[2], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_RIGHT_EXIST), args.tabArgs[2]);
        return ERROR;
    }
    fprintf(stdout, "%s: %s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_CHMOD), args.tabArgs[2]);
    return SUCCESS;
}

/**
 * @brief Executes listusers command
 *
 * @param args
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_listusers(cmd_t args) {
    if (args.nbArgs != 1) {
        terminal_print(LangGet(ERROR_COMMAND_LISTEUSERS_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_LISTEUSERS_USAGE));
        return ERROR;
    }
    fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_LISTUSERS));
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_users; ++i)
        fprintf(stdout, "%d : %s\n", i, virtual_disk_sos->users_table[i].login);
    return SUCCESS;
}

/**
 * @brief Executes adduser command
 *
 * @param args
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_adduser(cmd_t args, session_t user) {
    if (args.nbArgs != 1) {
        terminal_print(LangGet(ERROR_COMMAND_ADDUSER_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ADDUSER_USAGE));
        return ERROR;
    }
    if (user.userid != ROOT_UID) {
        terminal_print(LangGet(ERROR_COMMAND_ARGS_ROOT_ONLY), TERMINAL_RED);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ARGS_ROOT_ONLY));
        return ERROR;
    }

    char login[FILENAME_MAX_SIZE];
    char password[FILENAME_MAX_SIZE];
    char verifpwd[FILENAME_MAX_SIZE];
 
    fprintf(stdout, "%s ", LangGet(OUTPUT_COMMAND_ADDUSER_LOGIN));
    if (read_cmd(login, FILENAME_MAX_SIZE) == ERROR)
        return ERROR;

    if (strcmp(login, "") == 0) {
        terminal_print(LangGet(ERROR_COMMAND_ARGS_LOGIN_EMPTY), TERMINAL_RED);
        return ERROR;
    }
    
    terminal_non_canonique();
    printf("\e[?25l");

    do {
        fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_ADDUSER_PWD));
        if (read_cmd(password, FILENAME_MAX_SIZE) == ERROR)
            return ERROR;
        fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_ADDUSER_PWD_CONFIRM));
        if (read_cmd(verifpwd, FILENAME_MAX_SIZE) == ERROR)
            return ERROR;
    } while (!is_valid_password(password, verifpwd));

    terminal_canonique();
    printf("\e[?25h");

    
    if (init_user(login, password) == ERROR)
        return ERROR;
    // fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_ADDUSER_CREATED));
    terminal_print(LangGet(OUTPUT_COMMAND_ADDUSER_CREATED), TERMINAL_GREEN);
    return SUCCESS;
}

/**
 * @brief Executes rmuser command
 *
 * @param args
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_rmuser(cmd_t args, session_t user, int index_user_real) {
    if (args.nbArgs != 2) {
        terminal_print(LangGet(ERROR_COMMAND_RMUSER_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_RMUSER_USAGE));
        return ERROR;
    }

    if (user.userid != ROOT_UID) {
        terminal_print(LangGet(ERROR_COMMAND_ARGS_ROOT_ONLY), TERMINAL_RED);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ARGS_ROOT_ONLY));
        return ERROR;
    }

    int index_login = is_login_in_users_table(args.tabArgs[1]);
    if (index_login == NB_USERS) {
        fprintf(stderr, "%s%s %s%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_ARGS_LOGIN_EXIST), args.tabArgs[1], TERMINAL_RESET);
        // fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_LOGIN_EXIST), args.tabArgs[1]);
        return ERROR;
    }
    
    if (index_user_real != -1 && strcmp(args.tabArgs[1], virtual_disk_sos->users_table[index_user_real].login) == 0) { // si l'utilisateur à supprimer est celui connecté
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ARGS_LOGIN_SELF));
        terminal_print(LangGet(ERROR_COMMAND_ARGS_LOGIN_SELF), TERMINAL_RED);
        return ERROR;
    }

    if (strcmp(args.tabArgs[1], virtual_disk_sos->users_table[ROOT_UID].login) == 0) { // si l'utilisateur à supprimer est le root
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ARGS_REMOVE_ROOT));
        terminal_print(LangGet(ERROR_COMMAND_ARGS_REMOVE_ROOT), TERMINAL_RED);
        return ERROR;
    }

    if (delete_user(index_login) == ERROR)
        return ERROR;
    fprintf(stdout, "%s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_RMUSER));
    return SUCCESS;
}

int cmd_clear() {
    terminal_clear();
    terminal_cursor(0,0);
    return SUCCESS;
}

/**
 * @brief Executes help command
 *
 * @param args
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_help(cmd_t args) {
    if (args.nbArgs < 1) {
        terminal_print(LangGet(ERROR_COMMAND_HELP_USAGE), TERMINAL_ORANGE);
        // fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_HELP_USAGE));
        return ERROR;
    }
    if (args.nbArgs == 1) {
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_HELP_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LS_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CAT_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_RM_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CR_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_EDIT_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LOAD_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_STORE_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CHOWN_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CHMOD_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LISTEUSERS_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_ADDUSER_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_RMUSER_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_SUDO_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LOGOUT_USAGE));
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CLEAR_USAGE));
        return SUCCESS;
    } else {
        if (strcmp(args.tabArgs[1], CMD_HELP) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_HELP_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_LS) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LS_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_CAT) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CAT_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_RM) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_RM_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_CR) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CR_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_EDIT) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_EDIT_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_LOAD) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LOAD_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_STORE) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_STORE_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_CHOWN) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CHOWN_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_CHMOD) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CHMOD_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_LISTUSERS) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LISTEUSERS_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_ADDUSER) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_ADDUSER_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_RMUSER) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_RMUSER_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_SUDO) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_SUDO_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_LOGOUT) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LOGOUT_USAGE));
        else if (strcmp(args.tabArgs[1], CMD_CLEAR) == 0)
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CLEAR_USAGE));
        else {
            terminal_print(LangGet(ERROR_COMMAND_ARGS_UNKNOWN), TERMINAL_RED);
            // fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_ARGS_UNKNOWN));
            return ERROR;
        }
        return SUCCESS;
    }
}

/**
 * @brief Interprete string cmd to args tab
 * @param cmd
 * @param args
 */
void interprete_cmd(char *cmd, cmd_t *args) {
    args->nbArgs = 1;
    int i = 0;
    while (cmd[i] != '\0' && cmd[i] != ' ') {
        args->tabArgs[0][i] = cmd[i];
        i++;
    }
    args->tabArgs[0][i] = '\0';

    if (cmd[i] != '\0') {
        int j = 1, k = 0;
        args->nbArgs++;
        while (cmd[i] == ' ')
            i++;
        while (cmd[i] != '\0') {
            if (cmd[i] == ' ') {
                while (cmd[i] == ' ')
                    i++;
                args->tabArgs[j][k] = '\0';
                args->nbArgs++;
                j++;
                k = 0;
            }
            args->tabArgs[j][k] = cmd[i];
            i++;
            k++;
        }
        args->tabArgs[j][k] = '\0';
    }
}

/**
 * @brief Executes the given command if exists
 *
 * @param args
 * @param user
 * @return int, Success code or error code depending on whether successful or failure
 */
int execute_cmd(cmd_t args, session_t user, int sudo_mode_user) {
    char cmd_name[CMD_MAX_SIZE];
    strcpy(cmd_name, args.tabArgs[0]);
    if (strcmp(cmd_name, CMD_HELP) == 0)
        return cmd_help(args);
    if (strcmp(cmd_name, CMD_LS) == 0)
        return cmd_ls(args, user);
    if (strcmp(cmd_name, CMD_CAT) == 0)
        return cmd_cat(args, user);
    if (strcmp(cmd_name, CMD_RM) == 0)
        return cmd_rm(args, user);
    if (strcmp(cmd_name, CMD_CR) == 0)
        return cmd_cr(args, user);
    if (strcmp(cmd_name, CMD_EDIT) == 0)
        return cmd_edit(args, user);
    if (strcmp(cmd_name, CMD_LOAD) == 0)
        return cmd_load(args, user);
    if (strcmp(cmd_name, CMD_STORE) == 0)
        return cmd_store(args, user);
    if (strcmp(cmd_name, CMD_CHOWN) == 0)
        return cmd_chown(args, user);
    if (strcmp(cmd_name, CMD_CHMOD) == 0)
        return cmd_chmod(args, user);
    if (strcmp(cmd_name, CMD_LISTUSERS) == 0)
        return cmd_listusers(args);
    if (strcmp(cmd_name, CMD_ADDUSER) == 0)
        return cmd_adduser(args, user);
    if (strcmp(cmd_name, CMD_RMUSER) == 0)
        return cmd_rmuser(args, user, sudo_mode_user);
    if (strcmp(cmd_name, CMD_LOGOUT) == 0)
        return SUCCESS;
    if (strcmp(cmd_name, CMD_SUDO) == 0)
        return SUCCESS;
    if (strcmp(cmd_name, CMD_CLEAR) == 0)
        return cmd_clear();
    fprintf(stdout, "%s%s \"%s\"%s\n", TERMINAL_RED, LangGet(ERROR_COMMAND_UNKNOWN), cmd_name, TERMINAL_RESET);
    return ERROR;
}

/**
 * @brief Shell for the user to interact with the system
 * @return int, Success code or error code depending on whether successful or failure
 */
int terminal_shell() {
    char cmd[CMD_MAX_SIZE];
    int user_id = NB_USERS;
    session_t user;
    int system_on = true;
    char log[FILENAME_MAX_SIZE];
    char pwd[FILENAME_MAX_SIZE];
    
    while (system_on) {
        if (user_id == NB_USERS) {
            do {
                fprintf(stdout, "%s ", LangGet(CONSOLE_LOGIN));
                if (read_cmd(log, FILENAME_MAX_SIZE) == ERROR)
                    return ERROR;
                fprintf(stdout, "%s\n", LangGet(CONSOLE_PWD));
                terminal_non_canonique();
                printf("\e[?25l");
                if (read_cmd(pwd, FILENAME_MAX_SIZE) == ERROR)
                    return ERROR;
                terminal_canonique();
                printf("\e[?25h");
                user_id = is_good_credentials(log, pwd);
            } while (user_id == NB_USERS);
            user.userid = user_id;
        }
        fprintf(stdout, "[%s]: ", virtual_disk_sos->users_table[user.userid].login);
        if (read_cmd(cmd, CMD_MAX_SIZE) == ERROR)
            return ERROR;
        cmd_t interpreted_cmd;
        interprete_cmd(cmd, &interpreted_cmd);
        if (strcmp(interpreted_cmd.tabArgs[0], CMD_SUDO) == 0){
            interpreted_cmd.nbArgs--;
            for (int i = 0; i < interpreted_cmd.nbArgs; i++)
                strcpy(interpreted_cmd.tabArgs[i], interpreted_cmd.tabArgs[i + 1]);
            if (user.userid == ROOT_UID)
                execute_cmd(interpreted_cmd, user, -1);
            else {
                fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_SUDO_ROOT_PWD_ASK));
                char password[FILENAME_MAX_SIZE];
                terminal_non_canonique();
                printf("\e[?25l");
                if (read_cmd(password, FILENAME_MAX_SIZE) == ERROR)
                    return ERROR;
                terminal_canonique();
                printf("\e[?25h");
                if (is_good_credentials(virtual_disk_sos->users_table[ROOT_UID].login, password) == ROOT_UID) {
                    int saved_user_id = user.userid;
                    user.userid = ROOT_UID;
                    execute_cmd(interpreted_cmd, user, saved_user_id);
                    user.userid = saved_user_id;
                } else {
                    terminal_print(LangGet(OUTPUT_COMMAND_SUDO_ROOT_PWD_FAIL), TERMINAL_RED);
                    // fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_SUDO_ROOT_PWD_FAIL));
                }
            }
        }
        else if (strcmp(interpreted_cmd.tabArgs[0], CMD_LOGOUT) == 0)
            user_id = NB_USERS;
        else if (strcmp(interpreted_cmd.tabArgs[0], CMD_QUIT) == 0)
            system_on = false;
        else
            execute_cmd(interpreted_cmd, user, -1);
    }
    return SUCCESS;
}
