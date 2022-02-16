/**
* \file layer5.c
 * \brief Source code for layer5 of the ScratchOs : shell and commands
 * \author HERZBERG Dwayne and BERLIN Florian
 * \version 0.1
 * \date 13 February 2022
*/

#include "../headers/layer1.h"
#include "../headers/layer2.h"
#include "..//headers/layer3.h"
#include "../headers/layer4.h"
#include "../headers/layer5.h"

/**
 * \brief Read user input
 * @param cmd
 * @param size
 */
void read_cmd(char *cmd, int size){
    fgets(cmd, size, stdin);
    int got_size = (int)strlen(cmd);
    if(got_size == size) while(getchar() != '\n');
    else cmd[size-1] = '\0';
    cmd[strcspn(cmd, "\n")] = 0;
}

/**
 * \brief Executes ls command
 * @param args
 * @param session
 * @return
 */
int cmd_ls(cmd_t args){
    int last_inode = get_unused_inode(virtual_disk_sos->inodes);
    if (args.nbArgs > 2){
        fprintf(stderr, "Usage: ls [-l]\n");
        return ERROR;
    }
    else if (args.nbArgs == 2){
        if(strcmp(args.tabArgs[1], "-l") != 0) {
            fprintf(stderr, "Unknown argument\n");
            fprintf(stderr, "Usage: ls [-l]\n");
            return ERROR;
        }
        else{
            for (int i = 0; i < last_inode; i++) {
                char uright[3];
                inode_t inode = virtual_disk_sos->inodes[i];
                if(inode.uid == user.userid){
                    if (inode.uright == rw) strcpy(uright, "--");
                    else if (inode.uright == rW) strcpy(uright, "-w");
                    else if (inode.uright == Rw) strcpy(uright, "r-");
                    else if (inode.uright == rW) strcpy(uright, "rw");
                }
                else{
                    if (inode.oright == rw) strcpy(uright, "--");
                    else if (inode.oright == rW) strcpy(uright, "-w");
                    else if (inode.oright == Rw) strcpy(uright, "r-");
                    else if (inode.oright == rW) strcpy(uright, "rw");
                }
                fprintf(stdout, "%s %d %s %d %s %s\n", uright, inode.uid, virtual_disk_sos->users_table[inode.uid].login, inode.size, inode.mtimestamp, inode.filename);
            }
            return SUCCESS;
        }
    }
    else{
        for (int i = 0; i < virtual_disk_sos->super_block.number_of_files; i++) {
            fprintf(stdout, "%s ", virtual_disk_sos->inodes[i].filename);
        }
        printf("\n");
        return SUCCESS;
    }
}

/**
 * \brief Executes cat command
 * @param args
 * @param session
 * @return
 */
int cmd_cat(cmd_t args){
    if (args.nbArgs != 2){
        fprintf(stderr, "Usage: cat <file name>\n");
        return ERROR;
    }
    fprintf(stdout, "%s\n", args.tabArgs[1]);
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){
        fprintf(stderr, "File does not exist: %s\n", args.tabArgs[1]);
        return ERROR;
    }
    if(!has_rights(index_inode, user.userid, Rw)){
        fprintf(stderr, "You aren't authorized to access this file\n");
        return ERROR;
    }
    file_t file;
    read_file(args.tabArgs[1], &file);
    fprintf(stdout, "file: %s\n", args.tabArgs[1]);
    fprintf(stdout, "%s", file.data);
    return SUCCESS;
}

/**
 * \brief Executes rm command
 * @param args
 * @param session
 * @return
 */
int cmd_rm(cmd_t args) {
    if (args.nbArgs != 2){
        fprintf(stderr, "Usage: rm <file name>\n");
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){
        fprintf(stderr, "File does not exist: %s\n", args.tabArgs[1]);
        return ERROR;
    }
    if(!has_rights(index_inode, user.userid, rW)){
        fprintf(stderr, "You aren't authorized to access this file");
        return ERROR;
    }
    delete_file(args.tabArgs[1]);
    fprintf(stderr, "%s deleted", args.tabArgs[1]);
    return SUCCESS;
}

int cmd_cr(cmd_t args){
    return SUCCESS;
}

int cmd_edit(cmd_t args){
    return SUCCESS;
}

int cmd_load(cmd_t args){
    if (args.nbArgs != 2){
        fprintf(stderr, "Usage: load <file name>\n");
        return ERROR;
    }
    int code = load_file_from_host(args.tabArgs[1]);
    if (code == 0){
        fprintf(stderr, "Error on loading file %s\n", args.tabArgs[1]);
        return ERROR;
    }
    return SUCCESS;
}

int cmd_store(cmd_t args){
    if (args.nbArgs != 2){
        fprintf(stderr, "Usage: store <file name>\n");
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){
        fprintf(stderr, "File does not exist: %s\n", args.tabArgs[1]);
        return ERROR;
    }
    if(!has_rights(index_inode, user.userid, RW)){
        fprintf(stderr, "You aren't authorized to access this file");
        return ERROR;
    }
    int code = store_file_to_host(args.tabArgs[1]);
    if (code == 0){
        fprintf(stderr, "Error on storing file: %s\n", args.tabArgs[1]);
        return ERROR;
    }
    return SUCCESS;
}

int cmd_chown(cmd_t args){
    if (args.nbArgs != 3){
        fprintf(stderr, "Usage: chown <file name> <login>\n");
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){
        fprintf(stderr, "File does not exist: %s\n", args.tabArgs[1]);
        return ERROR;
    }
    int index_login = is_login_in_users_table(args.tabArgs[2]);
    if (index_login == NB_USERS){
        fprintf(stderr, "Login does not exist: %s\n", args.tabArgs[2]);
        return ERROR;
    }
    if(!has_rights(index_inode, user.userid, RW)){
        fprintf(stderr, "You aren't authorized to access this file");
        return ERROR;
    }
    uint last_uid = virtual_disk_sos->inodes[index_inode].uid;
    virtual_disk_sos->inodes[index_inode].uid = index_login;
    fprintf(stderr, "Rights of %s changed from %s to %s\n", args.tabArgs[1], virtual_disk_sos->users_table[last_uid].login, args.tabArgs[2]);
    return SUCCESS;
}

int cmd_chmod(cmd_t args){
    if (args.nbArgs != 3){
        fprintf(stderr, "Usage: chmod <file name> <right>\n");
        return ERROR;
    }
    int index_inode = is_file_in_inode(args.tabArgs[1]);
    if(index_inode == INODE_TABLE_SIZE){
        fprintf(stderr, "File does not exist: %s\n", args.tabArgs[1]);
        return ERROR;
    }
    if(!has_rights(index_inode, user.userid, RW)){
        fprintf(stderr, "You aren't authorized to access this file");
        return ERROR;
    }
    if (strcmp(args.tabArgs[2], "rw") == 0 || strcmp(args.tabArgs[2], "0") == 0) virtual_disk_sos->inodes[index_inode].oright = rw;
    if (strcmp(args.tabArgs[2], "rW") == 0 || strcmp(args.tabArgs[2], "1") == 0) virtual_disk_sos->inodes[index_inode].oright = rW;
    if (strcmp(args.tabArgs[2], "Rw") == 0 || strcmp(args.tabArgs[2], "2") == 0) virtual_disk_sos->inodes[index_inode].oright = Rw;
    if (strcmp(args.tabArgs[2], "RW") == 0|| strcmp(args.tabArgs[2], "3") == 0) virtual_disk_sos->inodes[index_inode].oright = RW;
    else{
        fprintf(stderr, "Those rights does not exist: %s\n", args.tabArgs[2]);
        return ERROR;
    }
    return SUCCESS;
}

int cmd_listusers(cmd_t args){
    if (args.nbArgs != 1){
        fprintf(stderr, "Usage: listusers\n");
        return ERROR;
    }
    fprintf(stdout, "List of users: \n");
    for (int i = 0; i < virtual_disk_sos->super_block.number_of_users; ++i) {
        fprintf(stdout,"%d : %s\n", i, virtual_disk_sos->users_table[i].login);
    }
    return SUCCESS;
}

int cmd_adduser(cmd_t args){
    if (args.nbArgs != 1){
        fprintf(stderr, "Usage: adduser\n");
        return ERROR;
    }
    if (user.userid != 0) {
        fprintf(stderr, "Only root user is allowed\n");
        return ERROR;
    }
    char login[FILENAME_MAX_SIZE];
    char pwd[FILENAME_MAX_SIZE];
    char pwd_hash[SHA256_BLOCK_SIZE*2 + 1];
    fprintf(stdout, "Please enter a login for the new user: \n");
    read_cmd(login, FILENAME_MAX_SIZE);
    fprintf(stdout, "Please enter a password for the new user: \n");
    read_cmd(pwd,SHA256_BLOCK_SIZE*2 + 1);
    sha256ofString((BYTE *)pwd,pwd_hash);
    init_user(login, pwd_hash);
    fprintf(stdout, "New user created\n");
    return SUCCESS;
}

int cmd_rmuser(cmd_t args){
    if (args.nbArgs != 2){
        fprintf(stderr, "Usage: rmuser <login>\n");
        return ERROR;
    }
    if (user.userid != 0) {
        fprintf(stderr, "Only root user is allowed\n");
        return ERROR;
    }
    int index_login = is_login_in_users_table(args.tabArgs[1]);
    if (index_login == NB_USERS){
        fprintf(stderr, "Login does not exist: %s\n", args.tabArgs[1]);
        return ERROR;
    }
    delete_user(index_login);
    fprintf(stdout, "User %s removed\n", args.tabArgs[1]);
    return SUCCESS;
}

/**
 * \brief interprete string cmd to args tab
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
 * \brief executes the given command if exists
 * @param args
 * @param session
 */
int execute_cmd(cmd_t args){
    char cmd_name[8];
    strcpy(cmd_name, args.tabArgs[0]);
    if(!strcmp(cmd_name, CMD_LS)) return cmd_ls(args);
    if(!strcmp(cmd_name, CMD_CAT)) return cmd_cat(args);
    if(!strcmp(cmd_name, CMD_RM)) return cmd_rm(args);
    if(!strcmp(cmd_name, CMD_CR)) return cmd_cr(args);
    if(!strcmp(cmd_name, CMD_EDIT)) return cmd_edit(args);
    if(!strcmp(cmd_name, CMD_LOAD)) return cmd_load(args);
    if(!strcmp(cmd_name, CMD_STORE)) return cmd_store(args);
    if(!strcmp(cmd_name, CMD_CHOWN)) return cmd_chown(args);
    if(!strcmp(cmd_name, CMD_CHMOD)) return cmd_chmod(args);
    if(!strcmp(cmd_name, CMD_LISTUSERS)) return cmd_listusers(args);
    if(!strcmp(cmd_name, CMD_ADDUSER)) return cmd_adduser(args);
    if(!strcmp(cmd_name, CMD_RMUSER)) return cmd_rmuser(args);
    fprintf(stderr, "[%s] Unknown command \"%s\"\n", virtual_disk_sos->users_table[user.userid].login, cmd_name);
    return ERROR;

}

/**
 * \brief shell for the user to interact with the system
 * @return
 */
int terminal_shell(){
    char cmd[CMD_MAX_SIZE];
    int user_id;
    int system_on = true;
    char log[FILENAME_MAX_SIZE];
    char pwd[FILENAME_MAX_SIZE];
    do {
        fprintf(stdout, "login: \n");
        read_cmd(log, FILENAME_MAX_SIZE);
        fprintf(stdout, "password: \n");
        read_cmd(pwd, FILENAME_MAX_SIZE);
        user_id = is_good_credentials(log, pwd);
    }while(user_id == NB_USERS);
    user.userid = user_id;

    while(system_on){
        fprintf(stdout, "[%s]: ", virtual_disk_sos->users_table[user.userid].login);
        read_cmd(cmd, CMD_MAX_SIZE);
        fprintf(stdout, "\n");
        cmd_t interpreted_cmd;
        interprete_cmd(cmd, &interpreted_cmd);
        if (strcmp(interpreted_cmd.tabArgs[0], CMD_QUIT) == 0) system_on = false;
        else execute_cmd(interpreted_cmd);
    }
    return SUCCESS;
}
