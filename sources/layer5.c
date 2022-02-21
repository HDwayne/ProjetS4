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
int read_cmd(char *cmd, int size){
    fgets(cmd, size, stdin);
    if (ferror(stdin)) { fprintf(stderr, "%s\n", LangGet(ERROR_USER_INPUT)); return ERROR; }
    int got_size = (int)strlen(cmd);
    if(got_size == size) while(getchar() != '\n');
    else cmd[size-1] = '\0';
    cmd[strcspn(cmd, "\n")] = 0;
    return SUCCESS;
}

/**
 * @brief Diplay to conole all infos of one inode
 * 
 * @param inode 
 * @param user 
 */
void display_ls_l(inode_t inode, session_t user){
    char uright[3];
    if (inode.uid == user.userid) {
        if (inode.uright == rw) strcpy(uright, "--");
        else if (inode.uright == rW) strcpy(uright, "-w");
        else if (inode.uright == Rw) strcpy(uright, "r-");
        else if (inode.uright == RW) strcpy(uright, "rw");
    } else{
        if (inode.oright == rw) strcpy(uright, "--");
        else if (inode.oright == rW) strcpy(uright, "-w");
        else if (inode.oright == Rw) strcpy(uright, "r-");
        else if (inode.oright == RW) strcpy(uright, "rw");
    }
    fprintf(stdout, "%s %d %s %d %s %s\n", uright, inode.uid, virtual_disk_sos->users_table[inode.uid].login, inode.size, inode.mtimestamp, inode.filename);
}

/**
 * @brief Executes ls command
 * @param args
 * @param session
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_ls(cmd_t args, session_t user){
    int last_inode = get_unused_inode(virtual_disk_sos->inodes);
    if (args.nbArgs > 2){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_LS_USAGE)); return ERROR; }
    else if (args.nbArgs == 2){
        if(strcmp(args.tabArgs[1], "-l") != 0) {
            fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ARGS_UNKNOWN));
            fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_LS_USAGE));
            return ERROR;
        } else {
            for (int i = 0; i < last_inode; i++) {
                inode_t inode = virtual_disk_sos->inodes[i];
                display_ls_l(inode, user);
            }
        }
    } else {
        for (int i = 0; i < virtual_disk_sos->super_block.number_of_files; i++) {
            fprintf(stdout, "%s ", virtual_disk_sos->inodes[i].filename);
        }
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
int cmd_cat(cmd_t args, session_t user){
    if (args.nbArgs != 2){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_CAT_USAGE)); return ERROR; }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]); return ERROR; }
    if(!has_rights(index_inode, user.userid, Rw)){ fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS)); return ERROR; }
    file_t file;
    if (read_file(args.tabArgs[1], &file) == ERROR) return ERROR;
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
    if (args.nbArgs != 2){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_RM_USAGE)); return ERROR; }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if (index_inode == INODE_TABLE_SIZE){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]); return ERROR; }
    if (!has_rights(index_inode, user.userid, rW)){ fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS)); return ERROR; }
    if (delete_file(args.tabArgs[1]) == ERROR) return ERROR;
    fprintf(stderr, "%s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_RM));
    return SUCCESS;
}

/**
 * @brief Executes cr command
 * 
 * @param args 
 * @param user 
 * @return int, Success code or error code depending on whether successful or failure 
 */
int cmd_cr(cmd_t args, session_t user){
    if (args.nbArgs != 2){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_CR_USAGE)); return ERROR; }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode != INODE_TABLE_SIZE){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]); return ERROR; }
    file_t file;
    file.size = 1;
    file.data[0] = '\0';
    if (write_file(args.tabArgs[1], file, user) == ERROR) return ERROR;
    fprintf(stdout, "%s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_CR));
    return SUCCESS;
}

/**
 * @brief Executes edit command
 * 
 * @param args 
 * @param user 
 * @return int 
 */
int cmd_edit(cmd_t args, session_t user){
    if (args.nbArgs != 2){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_EDIT_USAGE)); return ERROR; }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]); return ERROR; }
    if (!has_rights(index_inode, user.userid, rW)){ fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS)); return ERROR; }
    file_t file;
    file.size = 0;
    char ligne[MAX_MSG];
    int i =0;
    fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_EDIT_EDITING));
    do {
        fprintf(stdout, "%s%d : ", LangGet(OUTPUT_COMMAND_EDIT_LINE), i);
        read_cmd(ligne, MAX_MSG);
        if (strlen(ligne) == MAX_MSG) {
            fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_EDIT_INPUT_MAX));
        }
        if(ligne[0] != '\0' && (strlen(ligne) + file.size) <= MAX_FILE_SIZE) {
            memcpy(file.data + file.size, ligne, strlen(ligne));
            file.size += strlen(ligne);
            memcpy(file.data + file.size, "\n", strlen("\n"));
            file.size += strlen("\n");
            i++;
        }
    } while(ligne[0] != '\0' && file.size < MAX_FILE_SIZE);
    if (file.size > MAX_FILE_SIZE){
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_EDIT_FILE_MAXSIZE));
        file.data[MAX_FILE_SIZE-1] = '\0';
    }
    file.data[file.size] = '\0';
    file.size=strlen((char *)file.data);

    if (write_file(args.tabArgs[1], file, user) == ERROR) return ERROR;
    fprintf(stdout, "%s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_EDIT_END));
    return SUCCESS;
}

/**
 * @brief Executes load command
 * 
 * @param args 
 * @param user 
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_load(cmd_t args, session_t user){
    if (args.nbArgs != 2){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_LOAD_USAGE)); return ERROR; }
    if (load_file_from_host(args.tabArgs[1], user) == ERROR){ 
        fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_LOAD_LOADING_FILE), args.tabArgs[1]); return ERROR; 
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
int cmd_store(cmd_t args, session_t user){
    if (args.nbArgs != 2){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_STORE_USAGE)); return ERROR; }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]); return ERROR; }
    if(!has_rights(index_inode, user.userid, RW)){ fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS)); return ERROR; }
    if (store_file_to_host(args.tabArgs[1]) == ERROR){
        fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_STORE_STORING_FILE), args.tabArgs[1]); return ERROR;
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
int cmd_chown(cmd_t args, session_t user){
    if (args.nbArgs != 3){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_CHOWN_USAGE)); return ERROR; }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]); return ERROR; }
    int index_login = is_login_in_users_table(args.tabArgs[2]);
    if (index_login == NB_USERS){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_LOGIN_EXIST), args.tabArgs[2]); return ERROR; }
    if(!has_rights(index_inode, user.userid, RW)){ fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS)); return ERROR; }
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
int cmd_chmod(cmd_t args, session_t user){
    if (args.nbArgs != 3){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_CHMOD_USAGE)); return ERROR; }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_FILE_EXIST), args.tabArgs[1]); return ERROR; }
    if(!has_rights(index_inode, user.userid, RW)){ fprintf(stderr, "%s\n", LangGet(ERROR_FILE_RIGHTS)); return ERROR; }
    if (strcmp(args.tabArgs[2], "rw") == 0 || strcmp(args.tabArgs[2], "0") == 0) virtual_disk_sos->inodes[index_inode].oright = rw;
    else if (strcmp(args.tabArgs[2], "rW") == 0 || strcmp(args.tabArgs[2], "1") == 0) virtual_disk_sos->inodes[index_inode].oright = rW;
    else if (strcmp(args.tabArgs[2], "Rw") == 0 || strcmp(args.tabArgs[2], "2") == 0) virtual_disk_sos->inodes[index_inode].oright = Rw;
    else if (strcmp(args.tabArgs[2], "RW") == 0|| strcmp(args.tabArgs[2], "3") == 0) virtual_disk_sos->inodes[index_inode].oright = RW;
    else{ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_RIGHT_EXIST), args.tabArgs[2]); return ERROR; }
    fprintf(stdout, "%s: %s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_CHMOD), args.tabArgs[2]);
    return SUCCESS;
}

/**
 * @brief Executes listusers command
 * 
 * @param args 
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_listusers(cmd_t args){
    if (args.nbArgs != 1){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_LISTEUSERS_USAGE)); return ERROR; }
    fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_LISTUSERS));
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_users; ++i) fprintf(stdout,"%d : %s\n", i, virtual_disk_sos->users_table[i].login);
    return SUCCESS;
}

/**
 * @brief Executes adduser command
 * 
 * @param args 
 * @param user 
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_adduser(cmd_t args, session_t user){
    if (args.nbArgs != 1){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ADDUSER_USAGE)); return ERROR; }
    if (user.userid != ROOT_UID) { fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ARGS_ROOT_ONLY)); return ERROR; }
    char login[FILENAME_MAX_SIZE];
    char pwd[FILENAME_MAX_SIZE];
    fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_ADDUSER_LOGIN));
    if (read_cmd(login, FILENAME_MAX_SIZE) == ERROR) return ERROR;
    fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_ADDUSER_PWD));
    if (read_cmd(pwd,FILENAME_MAX_SIZE) == ERROR) return ERROR;
    if (init_user(login, pwd) == ERROR) return ERROR;
    fprintf(stdout, "%s\n", LangGet(OUTPUT_COMMAND_ADDUSER_CREATED));
    return SUCCESS;
}

/**
 * @brief Executes rmuser command
 * 
 * @param args 
 * @param user 
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_rmuser(cmd_t args, session_t user){
    if (args.nbArgs != 2){ fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_RMUSER_USAGE)); return ERROR; }
    if (user.userid != ROOT_UID) { fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_ARGS_ROOT_ONLY)); return ERROR; }
    int index_login = is_login_in_users_table(args.tabArgs[1]);
    if (index_login == NB_USERS){ fprintf(stderr, "%s %s\n", LangGet(ERROR_COMMAND_ARGS_LOGIN_EXIST), args.tabArgs[1]); return ERROR; }
    if (delete_user(index_login) == ERROR) return ERROR;
    fprintf(stdout, "%s %s\n", args.tabArgs[1], LangGet(OUTPUT_COMMAND_RMUSER));
    return SUCCESS;
}

/**
 * @brief Executes help command
 * 
 * @param args 
 * @return int, Success code or error code depending on whether successful or failure
 */
int cmd_help(cmd_t args){
    if (args.nbArgs < 1) { fprintf(stderr, "%s\n", LangGet(ERROR_COMMAND_HELP_USAGE)); return ERROR; }
    if (args.nbArgs == 1){
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
        return SUCCESS;
    } else {      
        if(strcmp(args.tabArgs[1], CMD_HELP) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_HELP_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_LS) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LS_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_CAT) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CAT_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_RM) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_RM_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_CR) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CR_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_EDIT) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_EDIT_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_LOAD) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LOAD_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_STORE) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_STORE_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_CHOWN) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CHOWN_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_CHMOD) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_CHMOD_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_LISTUSERS) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_LISTEUSERS_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_ADDUSER) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_ADDUSER_USAGE)); return SUCCESS; }
        if(strcmp(args.tabArgs[1], CMD_RMUSER) == 0) { fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_RMUSER_USAGE)); return SUCCESS; }
        fprintf(stdout, "%s\n", LangGet(ERROR_COMMAND_ARGS_UNKNOWN)); return ERROR;
    }
}

/**
 * @brief Interprete string cmd to args tab
 * @param cmd
 * @param args
 */
void interprete_cmd(char * cmd, cmd_t *args){
    args->nbArgs = 1;
    int i = 0;
    while(cmd[i] != '\0' && cmd[i] != ' '){
        args->tabArgs[0][i] = cmd[i];
        i++;
    }
    args->tabArgs[0][i] = '\0';

    if (cmd[i] != '\0'){
        int j = 1,  k = 0;
        args->nbArgs++;
        while (cmd[i] == ' ') i++;
        while (cmd[i] != '\0'){
            if (cmd[i] == ' '){
                while (cmd[i] == ' ') i++;
                args->tabArgs[j][k] = '\0';
                args->nbArgs++;
                j++;
                k=0;
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
int execute_cmd(cmd_t args, session_t user){
    char cmd_name[CMD_MAX_SIZE];
    strcpy(cmd_name, args.tabArgs[0]);
    if(strcmp(cmd_name, CMD_HELP) == 0) return cmd_help(args);
    if(strcmp(cmd_name, CMD_LS) == 0) return cmd_ls(args, user);
    if(strcmp(cmd_name, CMD_CAT) == 0) return cmd_cat(args, user);
    if(strcmp(cmd_name, CMD_RM) == 0) return cmd_rm(args, user);
    if(strcmp(cmd_name, CMD_CR) == 0) return cmd_cr(args, user);
    if(strcmp(cmd_name, CMD_EDIT) == 0) return cmd_edit(args, user);
    if(strcmp(cmd_name, CMD_LOAD) == 0) return cmd_load(args, user);
    if(strcmp(cmd_name, CMD_STORE) == 0) return cmd_store(args, user);
    if(strcmp(cmd_name, CMD_CHOWN) == 0) return cmd_chown(args, user);
    if(strcmp(cmd_name, CMD_CHMOD) == 0) return cmd_chmod(args, user);
    if(strcmp(cmd_name, CMD_LISTUSERS) == 0) return cmd_listusers(args);
    if(strcmp(cmd_name, CMD_ADDUSER) == 0) return cmd_adduser(args, user);
    if(strcmp(cmd_name, CMD_RMUSER) == 0) return cmd_rmuser(args, user);
    fprintf(stdout, "[%s] %s \"%s\"\n", virtual_disk_sos->users_table[user.userid].login, LangGet(ERROR_COMMAND_UNKNOWN), cmd_name);
    return ERROR;
}

/**
 * @brief Shell for the user to interact with the system 
 * @return int, Success code or error code depending on whether successful or failure
 */
int terminal_shell(){
    char cmd[CMD_MAX_SIZE];
    int user_id;
    session_t user;
    int system_on = true;
    char log[FILENAME_MAX_SIZE];
    char pwd[FILENAME_MAX_SIZE];
    do {
        fprintf(stdout, "%s\n", LangGet(CONSOLE_LOGIN));
        if (read_cmd(log, FILENAME_MAX_SIZE) == ERROR) return ERROR;
        fprintf(stdout, "%s\n", LangGet(CONSOLE_PWD));
        if (read_cmd(pwd, FILENAME_MAX_SIZE) == ERROR) return ERROR;
        user_id = is_good_credentials(log, pwd);
    } while(user_id == NB_USERS);
    user.userid = user_id;

    while(system_on){
        fprintf(stdout, "[%s]: ", virtual_disk_sos->users_table[user.userid].login);
        if (read_cmd(cmd, CMD_MAX_SIZE) == ERROR) return ERROR;
        cmd_t interpreted_cmd;
        interprete_cmd(cmd, &interpreted_cmd);
        if (strcmp(interpreted_cmd.tabArgs[0], CMD_QUIT) == 0) system_on = false;
        else { execute_cmd(interpreted_cmd, user); }
    }
    return SUCCESS;
}
